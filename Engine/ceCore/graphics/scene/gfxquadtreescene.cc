
#include <ceCore/graphics/scene/gfxquadtreescene.hh>
#include <ceCore/graphics/scene/gfxcamera.hh>
#include <ceCore/graphics/scene/gfxmesh.hh>
#include <ceCore/graphics/scene/gfxlight.hh>
#include <ceCore/graphics/scene/gfxscenecollector.hh>
#include <ceCore/graphics/material/imaterial.hh>
#include <ceCore/graphics/ilight.hh>
#include <ceCore/graphics/ipointlight.hh>
#include <ceCore/math/boundingbox.hh>
#include <ceCore/math/iclipper.hh>
#include <ceCore/math/vector3f.hh>
#include <array>
#include <algorithm>


namespace ce
{


static const unsigned MAX_ENTRIES_PER_CELL = 20;
static const unsigned MAX_CELL_DEPTH       = 4;


struct GfxQuadtreeScene::Cell
{

  Cell(Cell *parent, size_t depth, const Vector2f &min, const Vector2f &max);

  std::vector<GfxMesh *> m_shaded;
  std::vector<GfxMesh *> m_unshaded;


  void Add(GfxMesh *mesh);
  bool Remove(GfxMesh *mesh);
  void Decimate();
  CE_NODISCARD size_t Idx(const Vector3f &v) const;
  void UpdateBoundingBox();
  void RemoveLight(GfxLight *light) const;
  void ScanMeshes(const iClipper *clipper, GfxSceneCollector &collector) const;
  void ScanMeshes(const iClipper *clipper, uint32_t scanMask, GfxSceneCollector &collector) const;
  void ScanMeshes(const iClipper *clipper,
                  uint32_t scanMask,
                  const std::function<void(GfxMesh *)> &callback) const;
  Cell                   *m_parent;
  size_t                 m_depth;
  Vector2f               m_min;
  Vector2f               m_center;
  Vector2f               m_max;
  BoundingBox            m_bbox;
  std::array<Cell *, 4>  m_cells;
};

GfxQuadtreeScene::GfxQuadtreeScene()
{
  CE_CLASS_GEN_CONSTR;
  m_root = new Cell(nullptr, 0, Vector2f(-1000.0f, -1000.0f), Vector2f(1000.0f, 1000.0f));
}

void GfxQuadtreeScene::Add(GfxCamera *camera)
{
  if (std::ranges::find(m_cameras, camera) == m_cameras.end())
  {
    m_cameras.emplace_back(camera);
    camera->AddRef();
  }
}

void GfxQuadtreeScene::Remove(GfxCamera *camera)
{
  auto it = std::ranges::find(m_cameras, camera);
  if (it != m_cameras.end())
  {
    m_cameras.erase(it);
    camera->Release();
  }
}

void GfxQuadtreeScene::Add(GfxMesh *mesh)
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

void GfxQuadtreeScene::Remove(GfxMesh *mesh)
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

void GfxQuadtreeScene::Add(GfxLight *light)
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

void GfxQuadtreeScene::Add(GfxLight *light, std::vector<GfxLight *> &lights)
{
  if (std::find(lights.begin(), lights.end(), light) != lights.end())
  {
    return;
  }
  light->AddRef();
  lights.push_back(light);


}


void GfxQuadtreeScene::Remove(GfxLight *light)
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


void GfxQuadtreeScene::Remove(GfxLight *light, std::vector<GfxLight *> &lights)
{
  auto it = std::find(lights.begin(), lights.end(), light);
  if (it == lights.end())
  {
    return;
  }

  lights.erase(it);
  light->Release();
}


const std::vector<GfxCamera *> &GfxQuadtreeScene::GetCameras() const
{
  return m_cameras;
}

void GfxQuadtreeScene::ScanMeshes(const iClipper *clipper,
                                  uint32_t scanMask,
                                  const std::function<void(GfxMesh *)> &callback
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


void GfxQuadtreeScene::ScanMeshes(const iClipper *clipper, GfxSceneCollector &collector) const
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

void GfxQuadtreeScene::ScanMeshes(const iClipper *clipper, uint32_t scanMask, GfxSceneCollector &collector) const
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


void GfxQuadtreeScene::ScanGlobalLights(const std::function<bool(GfxLight *)> &callback) const
{
  for (auto light: m_globalLights)
  {
    if (!callback(light))
    {
      break;
    }
  }
}

void GfxQuadtreeScene::ScanStaticLights(const iClipper *clipper, const std::function<bool(GfxLight *)> &callback) const
{
  for (auto light: m_staticLights)
  {
    const iLight *lght = light->GetLight();
    bool         test  = true;
    if (lght->GetType() == eLT_Point)
    {
      auto *plight = lght->Query<const iPointLight>();
      test = clipper->Test(Sphere(plight->GetPosition(), plight->GetRange())) != eClippingResult::eCR_Outside;
    }
    if (test && !callback(light))
    {
      break;
    }
  }
}

void GfxQuadtreeScene::ScanDynamicLights(const iClipper *clipper, const std::function<bool(GfxLight *)> &callback) const
{
  for (auto light: m_dynamicLights)
  {
    const iLight *lght = light->GetLight();
    bool         test  = true;
    if (lght->GetType() == eLT_Point)
    {
      auto *plight = lght->Query<const iPointLight>();
      test = clipper->Test(Sphere(plight->GetPosition(), plight->GetRange())) != eClippingResult::eCR_Outside;
    }

    if (test && !callback(light))
    {
      break;
    }
  }
}

void GfxQuadtreeScene::ScanLights(const iClipper *clipper,
                                  uint32_t scanMask,
                                  const std::function<bool(GfxLight *)> &callback) const
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

GfxQuadtreeScene::Cell::Cell(Cell *parent, size_t depth, const Vector2f &min, const Vector2f &max)
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
void GfxQuadtreeScene::Cell::Add(GfxMesh *mesh)
{
  if (!mesh->GetMaterial())
  {
    return;
  }

  if (m_shaded.size() + m_unshaded.size() + 1 > MAX_ENTRIES_PER_CELL && m_depth < MAX_CELL_DEPTH)
  {
    m_cells[0] = new Cell(this, m_depth + 1, Vector2f(m_min.x, m_min.y), Vector2f(m_center.x, m_center.y));
    m_cells[1] = new Cell(this, m_depth + 1, Vector2f(m_min.x, m_center.y), Vector2f(m_center.x, m_max.y));
    m_cells[2] = new Cell(this, m_depth + 1, Vector2f(m_center.x, m_min.y), Vector2f(m_max.x, m_center.y));
    m_cells[3] = new Cell(this, m_depth + 1, Vector2f(m_center.x, m_center.y), Vector2f(m_max.x, m_max.y));

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
      if (std::ranges::find(m_unshaded, mesh) != m_unshaded.end())
      {
        return;
      }
      m_unshaded.emplace_back(mesh);
    }
    else
    {
      if (std::ranges::find(m_shaded, mesh) != m_shaded.end())
      {
        return;
      }
      m_shaded.emplace_back(mesh);
    }
    mesh->AddRef();
    mesh->ClearLights();
    mesh->SetLightingDirty(true);

    UpdateBoundingBox();
  }

}

bool GfxQuadtreeScene::Cell::Remove(GfxMesh *mesh)
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

void GfxQuadtreeScene::Cell::Decimate()
{
  // reduce the cell hierarchy;
}

void GfxQuadtreeScene::Cell::RemoveLight(GfxLight *light) const
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

size_t GfxQuadtreeScene::Cell::Idx(const Vector3f &v) const
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

void GfxQuadtreeScene::Cell::UpdateBoundingBox()
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
  for (const auto &item: m_unshaded)
  {
    m_bbox.Add(item->GetBoundingBox());
  }
  m_bbox.Finish();

  if (m_parent)
  {
    m_parent->UpdateBoundingBox();
  }
}

void GfxQuadtreeScene::Cell::ScanMeshes(const iClipper *clipper, GfxSceneCollector &collector) const
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
void GfxQuadtreeScene::Cell::ScanMeshes(const iClipper *clipper,
                                        uint32_t scanMask,
                                        GfxSceneCollector &collector
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
void GfxQuadtreeScene::Cell::ScanMeshes(const iClipper *clipper,
                                        uint32_t scanMask,
                                        const std::function<void(GfxMesh *)> &callback
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
