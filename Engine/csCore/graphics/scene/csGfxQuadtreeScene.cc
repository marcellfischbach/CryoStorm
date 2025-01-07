
#include <csCore/graphics/scene/csGfxQuadtreeScene.hh>
#include <csCore/graphics/scene/csGfxCamera.hh>
#include <csCore/graphics/scene/csGfxMeshCompound.hh>
#include <csCore/graphics/scene/csGfxMesh.hh>
#include <csCore/graphics/scene/csGfxLight.hh>
#include <csCore/graphics/scene/csGfxSceneCollector.hh>
#include <csCore/graphics/material/iMaterial.hh>
#include <csCore/graphics/iLight.hh>
#include <csCore/graphics/iPointLight.hh>
#include <csCore/math/csBoundingBox.hh>
#include <csCore/math/iclipper.hh>
#include <csCore/math/csVector3f.hh>
#include <csCore/csRef.hh>
#include <array>
#include <algorithm>


namespace cs
{

static const unsigned MAX_ENTRIES_PER_CELL = 20;
static const unsigned MAX_CELL_DEPTH       = 6;

static const bool OPTIMIZE = false;

struct MaterialCompound
{
  csRes<iMaterial> material;
  csGfxMeshCompound *mesh;
};

struct csGfxQuadtreeScene::Cell
{

  Cell(Cell *parent, size_t depth, const csVector2f &min, const csVector2f &max);

  bool m_dirty;

  std::vector<csGfxMesh *> m_shaded;
  std::vector<csGfxMesh *> m_unshaded;

  std::vector<csGfxMesh *> m_pendingShaded;
  std::vector<csGfxMesh *> m_pendingUnshaded;

  std::vector<MaterialCompound> m_shadedCompound;
  std::vector<MaterialCompound> m_unshadedCompound;


  void Add(csGfxMesh *mesh);
  bool Remove(csGfxMesh *mesh);
  void Decimate();

  void Optimize();
  void FlagOptimizationDirty();
  void AddShadedMesh(csGfxMesh *mesh);
  void AddUnshadedMesh(csGfxMesh *mesh);
  bool ContainsShaded(csGfxMesh *mesh);
  bool ContainsUnshaded(csGfxMesh *mesh);

  MaterialCompound& GetShadedCompound(csRes<iMaterial> & material);


  CS_NODISCARD size_t Idx(const csVector3f &v) const;
  void UpdateBoundingBox();
  void RemoveLight(csGfxLight *light) const;
  void ScanMeshes(const iClipper *clipper, csGfxSceneCollector &collector) const;
  void ScanMeshes(const iClipper *clipper, uint32_t scanMask, csGfxSceneCollector &collector) const;
  void ScanMeshes(const iClipper *clipper,
                  uint32_t scanMask,
                  const std::function<void(csGfxMesh *)> &callback) const;
  Cell                          *m_parent;
  size_t        m_depth;
  csVector2f    m_min;
  csVector2f    m_center;
  csVector2f    m_max;
  csBoundingBox m_bbox;
  std::array<Cell *, 4> m_cells;
  bool                          m_optimizationDirty = true;
};

csGfxQuadtreeScene::csGfxQuadtreeScene()
{

  m_root = new Cell(nullptr, 0, csVector2f(-100.0f, -100.0f), csVector2f(100.0f, 100.0f));
}

void csGfxQuadtreeScene::Add(csGfxCamera *camera)
{
  if (std::ranges::find(m_cameras, camera) == m_cameras.end())
  {
    m_cameras.emplace_back(camera);
    camera->AddRef();
  }
}

void csGfxQuadtreeScene::Remove(csGfxCamera *camera)
{
  auto it = std::ranges::find(m_cameras, camera);
  if (it != m_cameras.end())
  {
    m_cameras.erase(it);
    camera->Release();
  }
}

void csGfxQuadtreeScene::Add(csGfxMesh *mesh)
{
  if (!mesh->GetMaterial())
  {
    return;
  }

  if (mesh->IsStatic())
  {
    m_root->Add(mesh);
  }
  else if (mesh->GetMaterial()->GetShadingMode() == eShadingMode::Unshaded)
  {
    if (std::ranges::find(m_unshadedMeshes, mesh) != m_unshadedMeshes.end())
    {
      return;
    }
    m_unshadedMeshes.emplace_back(mesh);
    mesh->AddRef();
  }
  else
  {
    if (std::ranges::find(m_shadedMeshes, mesh) != m_shadedMeshes.end())
    {
      return;
    }
    m_shadedMeshes.emplace_back(mesh);
    mesh->AddRef();
  }


  mesh->AddRef();
  mesh->ClearLights();
  mesh->SetLightingDirty(true);

}

void csGfxQuadtreeScene::Remove(csGfxMesh *mesh)
{

  if (mesh->IsStatic())
  {
    m_root->Remove(mesh);
  }

  auto it = std::ranges::find(m_shadedMeshes, mesh);
  if (it != m_shadedMeshes.end())
  {
    m_shadedMeshes.erase(it);
    mesh->Release();
  }

  it = std::ranges::find(m_unshadedMeshes, mesh);
  if (it != m_unshadedMeshes.end())
  {
    m_unshadedMeshes.erase(it);
    mesh->Release();
  }

}

void csGfxQuadtreeScene::Add(csGfxLight *light)
{
  if (light)
  {
    if (light->GetLight()->GetType() == eLT_Directional)
    {
      Add(light, m_globalLights);
    }
    else if (light->IsStatic())
    {
      Add(light, m_staticLights);
    }
    else
    {
      Add(light, m_dynamicLights);
    }
  }
}

void csGfxQuadtreeScene::Add(csGfxLight *light, std::vector<csGfxLight *> &lights)
{
  if (std::ranges::find(lights.begin(), lights.end(), light) != lights.end())
  {
    return;
  }
  light->AddRef();
  lights.push_back(light);


}


void csGfxQuadtreeScene::Remove(csGfxLight *light)
{
  if (light)
  {
    m_root->RemoveLight(light);
    if (light->GetLight()->GetType() == eLT_Directional)
    {
      Remove(light, m_globalLights);
    }
    else if (light->IsStatic())
    {
      Remove(light, m_staticLights);
    }
    else
    {
      Remove(light, m_dynamicLights);
    }

  }
}


void csGfxQuadtreeScene::Remove(csGfxLight *light, std::vector<csGfxLight *> &lights)
{
  auto it = std::ranges::find(lights.begin(), lights.end(), light);
  if (it == lights.end())
  {
    return;
  }

  lights.erase(it);
  light->Release();
}


const std::vector<csGfxCamera *> &csGfxQuadtreeScene::GetCameras() const
{
  return m_cameras;
}

void csGfxQuadtreeScene::Optimize()
{
  m_root->Optimize();
}

void csGfxQuadtreeScene::ScanMeshes(const iClipper *clipper,
                                    uint32_t scanMask,
                                    const std::function<void(csGfxMesh *)> &callback
                                   ) const
{
  if (scanMask & eSM_Dynamic)
  {
    for (auto mesh: m_shadedMeshes)
    {
      if (!clipper || clipper->Test(mesh->GetBoundingBox()) != eClippingResult::eCR_Outside)
      {
        callback(mesh);
      }
    }
  }
  if (scanMask & eSM_Unshaded)
  {
    for (auto mesh: m_unshadedMeshes)
    {
      if (!clipper || clipper->Test(mesh->GetBoundingBox()) != eClippingResult::eCR_Outside)
      {
        callback(mesh);
      }
    }
  }

  if (scanMask & (eSM_Static | eSM_Unshaded))
  {
    m_root->ScanMeshes(clipper, scanMask, callback);
  }
}


void csGfxQuadtreeScene::ScanMeshes(const iClipper *clipper, csGfxSceneCollector &collector) const
{
  for (auto mesh: m_shadedMeshes)
  {
    if (!clipper || clipper->Test(mesh->GetBoundingBox()) != eClippingResult::eCR_Outside)
    {
      collector.AddMesh(mesh);
    }
  }

  for (auto mesh: m_unshadedMeshes)
  {
    if (!clipper || clipper->Test(mesh->GetBoundingBox()) != eClippingResult::eCR_Outside)
    {
      collector.AddMesh(mesh);
    }
  }
  m_root->ScanMeshes(clipper, collector);
}

void csGfxQuadtreeScene::ScanMeshes(const iClipper *clipper, uint32_t scanMask, csGfxSceneCollector &collector) const
{
  if (scanMask & eSM_Dynamic)
  {
    for (auto mesh: m_shadedMeshes)
    {
      if (!clipper || clipper->Test(mesh->GetBoundingBox()) != eClippingResult::eCR_Outside)
      {
        collector.AddMesh(mesh);
      }
    }
  }
  if (scanMask & eSM_Unshaded)
  {
    for (auto mesh: m_unshadedMeshes)
    {
      if (!clipper || clipper->Test(mesh->GetBoundingBox()) != eClippingResult::eCR_Outside)
      {
        collector.AddMesh(mesh);
      }
    }
  }

  if (scanMask & (eSM_Static | eSM_Unshaded))
  {
    m_root->ScanMeshes(clipper, scanMask, collector);
  }
}


void csGfxQuadtreeScene::ScanGlobalLights(const std::function<bool(csGfxLight *)> &callback) const
{
  for (auto light: m_globalLights)
  {
    if (!callback(light))
    {
      break;
    }
  }
}

void csGfxQuadtreeScene::ScanStaticLights(const iClipper *clipper, const std::function<bool(csGfxLight *)> &callback) const
{
  for (auto light: m_staticLights)
  {
    const iLight *lght = light->GetLight();
    bool         test  = true;
    if (lght->GetType() == eLT_Point)
    {
      auto *plight = lght->Query<const iPointLight>();
      test = clipper->Test(csSphere(plight->GetPosition(), plight->GetRange())) != eClippingResult::eCR_Outside;
    }
    if (test && !callback(light))
    {
      break;
    }
  }
}

void csGfxQuadtreeScene::ScanDynamicLights(const iClipper *clipper, const std::function<bool(csGfxLight *)> &callback) const
{
  for (auto light: m_dynamicLights)
  {
    const iLight *lght = light->GetLight();
    bool         test  = true;
    if (lght->GetType() == eLT_Point)
    {
      auto *plight = lght->Query<const iPointLight>();
      test = clipper->Test(csSphere(plight->GetPosition(), plight->GetRange())) != eClippingResult::eCR_Outside;
    }

    if (test && !callback(light))
    {
      break;
    }
  }
}

void csGfxQuadtreeScene::ScanLights(const iClipper *clipper,
                                    uint32_t scanMask,
                                    const std::function<bool(csGfxLight *)> &callback) const
{
  if (scanMask & eSM_Global)
  {
    ScanGlobalLights(callback);
  }

  if (scanMask & eSM_Static)
  {
    ScanStaticLights(clipper, callback);
  }

  if (scanMask & eSM_Dynamic)
  {
    ScanDynamicLights(clipper, callback);
  }
}

csGfxQuadtreeScene::Cell::Cell(Cell *parent, size_t depth, const csVector2f &min, const csVector2f &max)
    : m_parent(parent)
    , m_depth(depth)
    , m_min(min)
    , m_max(max)
    , m_center((min + max) / 2.0f)
    , m_cells(std::array<Cell *, 4> {})
{
  m_bbox.Clear();
  m_bbox.Finish();
}


/*  y
 *  A
 *  |  +-------+-------+
 *  |  |       |       |
 *  |  |   1   |   3   |
 *  |  |       |       |
 *  |  +-------+-------+
 *  |  |       |       |
 *  |  |   0   |   2   |
 *  |  |       |       |
 *  |  +-------+-------+
 *  |
 *  +---------------------> x
 *
 *
 */
void csGfxQuadtreeScene::Cell::Add(csGfxMesh *mesh)
{
  if (!mesh->GetMaterial())
  {
    return;
  }

  if (m_shaded.size() + m_unshaded.size() + 1 > MAX_ENTRIES_PER_CELL && m_depth < MAX_CELL_DEPTH)
  {
    m_cells[0] = new Cell(this, m_depth + 1, csVector2f(m_min.x, m_min.y), csVector2f(m_center.x, m_center.y));
    m_cells[1] = new Cell(this, m_depth + 1, csVector2f(m_min.x, m_center.y), csVector2f(m_center.x, m_max.y));
    m_cells[2] = new Cell(this, m_depth + 1, csVector2f(m_center.x, m_min.y), csVector2f(m_max.x, m_center.y));
    m_cells[3] = new Cell(this, m_depth + 1, csVector2f(m_center.x, m_center.y), csVector2f(m_max.x, m_max.y));

    for (const auto &item: m_shaded)
    {
      size_t idx = Idx(item->GetModelMatrix().GetTranslation());
      m_cells[idx]->Add(item);
      item->Release();
    }
    m_shaded.clear();
    for (const auto &item: m_unshaded)
    {
      size_t idx = Idx(item->GetModelMatrix().GetTranslation());
      m_cells[idx]->Add(item);
      item->Release();
    }
    m_unshaded.clear();
  }


  if (m_cells[0])
  {
    size_t idx = Idx(mesh->GetModelMatrix().GetTranslation());
    m_cells[idx]->Add(mesh);
  }
  else
  {
    if (mesh->GetMaterial()->GetShadingMode() == eShadingMode::Unshaded)
    {
      AddUnshadedMesh(mesh);
    }
    else
    {
      AddShadedMesh(mesh);
    }

  }

}

bool csGfxQuadtreeScene::Cell::Remove(csGfxMesh *mesh)
{
  auto it = std::ranges::find(m_shaded, mesh);
  if (it != m_shaded.end())
  {
    m_shaded.erase(it);
    mesh->Release();
    Decimate();
    return true;
  }
  it = std::ranges::find(m_unshaded, mesh);
  if (it != m_unshaded.end())
  {
    m_unshaded.erase(it);
    mesh->Release();
    Decimate();
    return true;
  }
  if (m_cells[0])
  {
    size_t idx = Idx(mesh->GetModelMatrix().GetTranslation());
    if (m_cells[idx]->Remove(mesh))
    {
      return true;
    }

    for (size_t i = 0; i < 4; i++)
    {
      if (i != idx)
      {
        if (m_cells[i]->Remove(mesh))
        {
          return true;
        }
      }
    }
  }

  return false;
}

void csGfxQuadtreeScene::Cell::Decimate()
{
  // reduce the cell hierarchy;
}

MaterialCompound &csGfxQuadtreeScene::Cell::GetShadedCompound(csRes<iMaterial> &material)
{
  for (auto &materialCompound: m_shadedCompound)
  {
    if (materialCompound.material == material)
    {
      return materialCompound;
    }
  }
  MaterialCompound &cmp = m_shadedCompound.emplace_back(MaterialCompound { material, new csGfxMeshCompound() });
  m_shaded.emplace_back(cmp.mesh);
  return cmp;
}

void csGfxQuadtreeScene::Cell::Optimize()
{
  if (!m_optimizationDirty)
  {
    return;
  }

  m_optimizationDirty = false;
  if (m_depth != MAX_CELL_DEPTH)
  {
    if (m_cells[0])
    {
      for (auto &m_cell: m_cells)
      {
        m_cell->Optimize();
      }
    }
    return;
  }

  std::set<csGfxMeshCompound*> compounds;
  for (auto pendingMesh: m_pendingShaded)
  {
    MaterialCompound &cmp = GetShadedCompound(pendingMesh->GetMaterial());
    cmp.mesh->AddMesh(pendingMesh);
    pendingMesh->Release();
    compounds.insert(cmp.mesh);
  }
  m_pendingShaded.clear();

  for (const auto &meshCompound: compounds)
  {
    meshCompound->RegenerateMesh();
  }
}

void csGfxQuadtreeScene::Cell::FlagOptimizationDirty()
{
  Cell *cell = this;
  while (cell)
  {
    if (cell->m_optimizationDirty)
    {
      return;
    }
    cell->m_optimizationDirty = true;
    cell = cell->m_parent;
  }
}

void csGfxQuadtreeScene::Cell::AddShadedMesh(cs::csGfxMesh *mesh)
{
  if (ContainsShaded(mesh))
  {
    return;
  }


//  if (m_depth == MAX_CELL_DEPTH && mesh->IsBatchable())
//  {
//    iMaterial       *material = mesh->GetMaterial();
//    for (const auto &compound: m_shadedCompound)
//    {
//      if (compound.material == material)
//      {
//        compound.mesh->AddMesh(mesh);
//        UpdateBoundingBox();
//        return;
//      }
//    }
//
//    MaterialCompound compound {
//        mesh->GetMaterial(),
//        new GfxMeshCompound()
//    };
//    compound.mesh->AddMesh(mesh);
//    m_shadedCompound.emplace_back(compound);
//
//    mesh = compound.mesh;
//  }
  if (m_depth == MAX_CELL_DEPTH && mesh->IsBatchable() && OPTIMIZE)
  {
    m_pendingShaded.emplace_back(mesh);
    FlagOptimizationDirty();
  }
  else
  {
    m_shaded.emplace_back(mesh);
  }

  mesh->AddRef();
  mesh->ClearLights();
  mesh->SetLightingDirty(true);
  UpdateBoundingBox();

}


void csGfxQuadtreeScene::Cell::AddUnshadedMesh(cs::csGfxMesh *mesh)
{
  if (ContainsUnshaded(mesh))
  {
    return;
  }

  m_unshaded.emplace_back(mesh);

  mesh->AddRef();
  mesh->ClearLights();
  mesh->SetLightingDirty(true);

  UpdateBoundingBox();
}

bool csGfxQuadtreeScene::Cell::ContainsShaded(cs::csGfxMesh *mesh)
{
  for (const auto &compound: m_shadedCompound)
  {
    if (compound.mesh && compound.mesh->ContainsMesh(mesh))
    {
      return true;
    }
  }
  return std::ranges::find(m_shaded, mesh) != m_shaded.end();
}

bool csGfxQuadtreeScene::Cell::ContainsUnshaded(cs::csGfxMesh *mesh)
{
  for (const auto &compound: m_unshadedCompound)
  {
    if (compound.mesh && compound.mesh->ContainsMesh(mesh))
    {
      return true;
    }
  }
  return std::ranges::find(m_unshaded, mesh) != m_unshaded.end();
}

void csGfxQuadtreeScene::Cell::RemoveLight(csGfxLight *light) const
{
  for (auto mesh: m_shaded)
  {
    mesh->RemoveLight(light);
  }
  for (auto mesh: m_unshaded)
  {
    mesh->RemoveLight(light);
  }

  if (m_cells[0])
  {
    for (auto m_cell: m_cells)
    {
      m_cell->RemoveLight(light);
    }
  }
}

size_t csGfxQuadtreeScene::Cell::Idx(const csVector3f &v) const
{
  size_t idx = 0;
  if (v.x > m_center.x)
  {
    idx += 2;
  }
  if (v.z > m_center.y)
  {
    idx += 1;
  }
  return idx;
}

void csGfxQuadtreeScene::Cell::UpdateBoundingBox()
{
  m_bbox.Clear();
  if (m_cells[0])
  {
    for (auto &m_cell: m_cells)
    {
      m_bbox.Add(m_cell->m_bbox);
    }
  }
  for (const auto &item: m_shaded)
  {
    m_bbox.Add(item->GetBoundingBox());
  }
  for (const auto &item: m_pendingShaded)
  {
    m_bbox.Add(item->GetBoundingBox());
  }
  for (const auto &item: m_unshaded)
  {
    m_bbox.Add(item->GetBoundingBox());
  }
  for (const auto &item: m_pendingUnshaded)
  {
    m_bbox.Add(item->GetBoundingBox());
  }
  m_bbox.Finish();

  if (m_parent)
  {
    m_parent->UpdateBoundingBox();
  }
}

void csGfxQuadtreeScene::Cell::ScanMeshes(const iClipper *clipper, csGfxSceneCollector &collector) const
{
  eClippingResult res = clipper
                        ? clipper->Test(m_bbox)
                        : eClippingResult::eCR_Inside;
  switch (res)
  {
    case eClippingResult::eCR_Outside:
      return;
    case eClippingResult::eCR_Inside:
      clipper = nullptr;
      break;
    default:
      break;
  }


  for (auto mesh: m_unshaded)
  {
    if (!clipper || clipper->Test(mesh->GetBoundingBox()) != eClippingResult::eCR_Outside)
    {
      collector.AddMesh(mesh);
    }
  }
  for (auto mesh: m_shaded)
  {
    if (!clipper || clipper->Test(mesh->GetBoundingBox()) != eClippingResult::eCR_Outside)
    {
      collector.AddMesh(mesh);
    }
  }
  if (m_cells[0])
  {
    for (auto m_cell: m_cells)
    {
      m_cell->ScanMeshes(clipper, collector);
    }
  }
}
void csGfxQuadtreeScene::Cell::ScanMeshes(const iClipper *clipper,
                                          uint32_t scanMask,
                                          csGfxSceneCollector &collector
                                         ) const
{
  eClippingResult res = clipper
                        ? clipper->Test(m_bbox)
                        : eClippingResult::eCR_Inside;
  switch (res)
  {
    case eClippingResult::eCR_Outside:
      return;
    case eClippingResult::eCR_Inside:
      clipper = nullptr;
      break;
    default:
      break;
  }


  if (scanMask & eSM_Unshaded)
  {
    for (auto mesh: m_unshaded)
    {
      if (!clipper || clipper->Test(mesh->GetBoundingBox()) != eClippingResult::eCR_Outside)
      {
        collector.AddMesh(mesh);
      }
    }
  }
  if (scanMask & eSM_Static)
  {
    for (auto mesh: m_shaded)
    {
      if (!clipper || clipper->Test(mesh->GetBoundingBox()) != eClippingResult::eCR_Outside)
      {
        collector.AddMesh(mesh);
      }
    }
  }
  if (m_cells[0])
  {
    for (auto m_cell: m_cells)
    {
      m_cell->ScanMeshes(clipper, scanMask, collector);
    }
  }
}
void csGfxQuadtreeScene::Cell::ScanMeshes(const iClipper *clipper,
                                          uint32_t scanMask,
                                          const std::function<void(csGfxMesh *)> &callback
                                         ) const
{
  eClippingResult res = clipper
                        ? clipper->Test(m_bbox)
                        : eClippingResult::eCR_Inside;
  switch (res)
  {
    case eClippingResult::eCR_Outside:
      return;
    case eClippingResult::eCR_Inside:
      clipper = nullptr;
      break;
    default:
      break;
  }


  if (scanMask & eSM_Unshaded)
  {
    for (auto mesh: m_unshaded)
    {
      if (!clipper || clipper->Test(mesh->GetBoundingBox()) != eClippingResult::eCR_Outside)
      {
        callback(mesh);
      }
    }
  }
  if (scanMask & eSM_Static)
  {
    for (auto mesh: m_shaded)
    {
      if (!clipper || clipper->Test(mesh->GetBoundingBox()) != eClippingResult::eCR_Outside)
      {
        callback(mesh);
      }
    }
  }
  if (m_cells[0])
  {
    for (auto m_cell: m_cells)
    {
      m_cell->ScanMeshes(clipper, scanMask, callback);
    }
  }
}

}
