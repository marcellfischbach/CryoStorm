
#include <csCore/graphics/scene/csGfxSimpleScene.hh>
#include <csCore/graphics/scene/csGfxCamera.hh>
#include <csCore/graphics/iLight.hh>
#include <csCore/graphics/iPointLight.hh>
#include <csCore/graphics/csMesh.hh>
#include <algorithm>


namespace cs
{

GfxSimpleScene::GfxSimpleScene()
        : iGfxScene()
{

}

void GfxSimpleScene::Add(csGfxCamera *camera)
{
  if (std::ranges::find(m_cameras, camera) == m_cameras.end())
  {
    m_cameras.emplace_back(camera);
    camera->AddRef();
  }
}

void GfxSimpleScene::Remove(csGfxCamera*camera)
{
  auto it = std::ranges::find(m_cameras, camera);
  if (it != m_cameras.end())
  {
    m_cameras.erase(it);
    camera->Release();
  }
}

void GfxSimpleScene::Add(csGfxMesh *mesh)
{
  if (!mesh)
  {
    return;
  }
  if (mesh->GetMaterial() && mesh->GetMaterial()->GetShadingMode() == eShadingMode::Unshaded)
  {
    if (std::ranges::find(m_unshadedMeshes, mesh) != m_unshadedMeshes.end())
    {
      return;
    }
    m_unshadedMeshes.emplace_back(mesh);
  }
  else if (mesh->IsStatic())
  {
    if (std::ranges::find(m_staticMeshes, mesh) != m_staticMeshes.end())
    {
      return;
    }
    m_staticMeshes.emplace_back(mesh);
  }
  else
  {
    if (std::ranges::find(m_dynamicMeshes, mesh) != m_dynamicMeshes.end())
    {
      return;
    }
    m_dynamicMeshes.emplace_back(mesh);
  }

  mesh->AddRef();
  mesh->ClearLights();
  mesh->SetLightingDirty(true);

}

void GfxSimpleScene::Remove(csGfxMesh *mesh)
{
  if (!mesh)
  {
    return;
  }

  mesh->ClearLights();

  if (!mesh->GetMaterial() || mesh->GetMaterial()->GetShadingMode() == eShadingMode::Unshaded)
  {
    auto it = std::ranges::find(m_unshadedMeshes, mesh);
    if (it != m_unshadedMeshes.end())
    {
      m_unshadedMeshes.erase(it);
      return;
    }
  }
  if (mesh->IsStatic())
  {
    auto it = std::ranges::find(m_staticMeshes, mesh);
    if (it != m_staticMeshes.end())
    {
      m_staticMeshes.erase(it);
      return;
    }
  }
  else
  {
    auto it = std::ranges::find(m_dynamicMeshes, mesh);
    if (it != m_dynamicMeshes.end())
    {
      m_dynamicMeshes.erase(it);
      return;
    }
  }


  mesh->Release();
}

void GfxSimpleScene::Add(csGfxLight *light)
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

void GfxSimpleScene::Add(csGfxLight *light, std::vector<csGfxLight *> &lights)
{
  if (std::ranges::find(lights.begin(), lights.end(), light) != lights.end())
  {
    return;
  }
  light->AddRef();
  lights.push_back(light);


}

void GfxSimpleScene::Remove(csGfxLight *light)
{
  if (light)
  {
    for (auto mesh: m_staticMeshes)
    {
      mesh->RemoveLight(light);
    }

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

void GfxSimpleScene::Remove(csGfxLight *light, std::vector<csGfxLight *> &lights)
{
  auto it = std::ranges::find(lights.begin(), lights.end(), light);
  if (it == lights.end())
  {
    return;
  }

  lights.erase(it);
  light->Release();
}


const std::vector<csGfxCamera*> &GfxSimpleScene::GetCameras() const
{
  return m_cameras;
}




void GfxSimpleScene::ScanMeshes(const iClipper *clipper,
                                uint32_t scanMask,
                                const std::function<void(csGfxMesh *)> &callback) const
{
  if (scanMask & eSM_Static)
  {
    for (auto mesh: m_staticMeshes)
    {
      if (!clipper || clipper->Test(mesh->GetBoundingBox()) != eClippingResult::eCR_Outside)
      {
        callback(mesh);
      }
    }
  }

  if (scanMask & eSM_Dynamic)
  {
    for (auto mesh: m_dynamicMeshes)
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

}

void GfxSimpleScene::ScanGlobalLights(const std::function<bool(csGfxLight *)> &callback) const
{
  for (auto light: m_globalLights)
  {
    if (!callback(light))
    {
      break;
    }
  }
}

void GfxSimpleScene::ScanStaticLights(const iClipper *clipper, const std::function<bool(csGfxLight *)> &callback) const
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

void GfxSimpleScene::ScanDynamicLights(const iClipper *clipper, const std::function<bool(csGfxLight *)> &callback) const
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

void GfxSimpleScene::ScanLights(const iClipper *clipper,
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

void GfxSimpleScene::ScanMeshes(const iClipper *clipper, csGfxSceneCollector &collector) const
{

}

void GfxSimpleScene::ScanMeshes(const iClipper *clipper, uint32_t scanMask, csGfxSceneCollector &collector) const
{

}


}