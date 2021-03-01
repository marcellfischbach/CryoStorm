
#include <spcOpenGL/gl4/pipeline/forward/gl4forwardpipeline.hh>
#include <spcOpenGL/gl4/gl4device.hh>
#include <spcOpenGL/gl4/gl4directionallight.hh>
#include <spcOpenGL/gl4/gl4pointlight.hh>
#include <spcOpenGL/gl4/gl4rendertargetcube.hh>
#include <spcCore/graphics/camera.hh>
#include <spcCore/graphics/projector.hh>
#include <spcCore/graphics/scene/gfxscene.hh>
#include <spcCore/graphics/irendertarget2d.hh>
#include <spcCore/graphics/irendertargetcube.hh>
#include <spcCore/graphics/isampler.hh>
#include <spcCore/math/clipper/boxclipper.hh>
#include <spcCore/math/clipper/sphereclipper.hh>
#include <algorithm>
#include <GL/glew.h>

namespace spc::opengl
{

const int MaxLights = 4;
const float MinLightInfluence = 0.0f;

GL4ForwardPipeline::GL4ForwardPipeline()
  : m_frame(0)
  , m_device(nullptr)
  , m_scene(nullptr)
  , m_target(nullptr)
  , m_shadowMapColorSampler(nullptr)
  , m_shadowMapDepthSampler(nullptr)
{
  SPC_CLASS_GEN_CONSTR;
}

GL4ForwardPipeline::~GL4ForwardPipeline() noexcept
{

}
void GL4ForwardPipeline::Render(iRenderTarget2D* target, Camera& camera, Projector& projector, iDevice* device, GfxScene* scene)
{
  ++m_frame;
  m_device = device;
  m_camera = camera;
  m_projector = projector;
  m_scene = scene;
  m_target = target;


  camera.Bind(device);
  projector.Bind(device);
  BoxClipper clipper(Vector3f(-1000.0f, -1000.0f, -1000.0f), Vector3f(1000.0f, 1000.0f, 1000.0f));


  m_shadowDirectionalLights.clear();
  m_shadowPointLights.clear();



  // get all global finalRenderLights from the scene
  const GfxLight* finalRenderLights[MaxLights];
  Size finalRenderLightOffset = 0;
  scene->ScanLights(&clipper, GfxScene::eSM_Global,
    [this, &finalRenderLights, &finalRenderLightOffset](GfxLight* light) {

      if (finalRenderLightOffset >= MaxLights)
      {
        return false;
      }
      finalRenderLights[finalRenderLightOffset++] = light;
      CollectShadowLights(light);
      return true;
    });


  m_dynamicLights.clear();
  m_staticLights.clear();
  m_staticLightsNew.clear();
  scene->ScanLights(&clipper, GfxScene::eSM_Dynamic | GfxScene::eSM_Static,
    [this](GfxLight* light)
    {
      LightScanned(light);
      CollectShadowLights(light);
      return true;
    });


  RenderShadowMaps();

  device->SetRenderTarget(m_target);
  device->Clear(true, spc::Color4f(0.0f, 0.0, 0.0, 1.0f), true, 1.0f, true, 0);

  scene->ScanMeshes(&clipper, GfxScene::eSM_Dynamic | GfxScene::eSM_Static,
    [this, &finalRenderLights, &finalRenderLightOffset](GfxMesh* mesh)
    {
      MeshScanned(mesh, finalRenderLights, finalRenderLightOffset);
    }
  );

  if (!m_pointLightShadowMap.empty())
  {
    GL4RenderTargetCube* cube = m_pointLightShadowMap[0];
    Size size = target->GetWidth() / 4;

    m_device->SetViewport(0, 0, size, size);
    m_device->RenderFullscreen(cube->GetColorTexture(0), 1);
    
    m_device->SetViewport(size, 0, size, size);
    m_device->RenderFullscreen(cube->GetColorTexture(0), 4);

    m_device->SetViewport(size * 2, 0, size, size);
    m_device->RenderFullscreen(cube->GetColorTexture(0), 0);

    m_device->SetViewport(size * 3, 0, size, size);
    m_device->RenderFullscreen(cube->GetColorTexture(0), 5);
    

  }

  //scene->Render(device, spc::eRP_Forward);
  m_device = nullptr;
  m_camera = camera;
  m_projector = projector;
  m_scene = nullptr;
  m_target = nullptr;

}



void GL4ForwardPipeline::LightScanned(GfxLight* light)
{
  if (light->IsStatic())
  {
    m_staticLights.push_back(light);
    if (light->GetFrame() == 0)
    {
      m_staticLightsNew.push_back(light);
      light->SetFrame(m_frame);
    }
  }
  else
  {
    m_dynamicLights.push_back(light);
  }
}



void GL4ForwardPipeline::MeshScanned(GfxMesh* mesh, const GfxLight** lights, Size offset)
{

  if (mesh->IsStatic())
  {
    if (mesh->IsLightingDirty())
    {
      mesh->ClearLights();
      AppendLights(mesh, m_staticLights);
      mesh->SetLightingDirty(false);
    }
    else
    {
      AppendLights(mesh, m_staticLightsNew);
    }

    auto dynamicLights = CalcMeshLightInfluences(mesh, m_dynamicLights, true);
    Size numLights = AssignLights(
      mesh->GetLights(),
      dynamicLights,
      lights,
      offset);
    mesh->RenderForward(m_device, eRP_Forward, lights, numLights);
  }
  else
  {
    auto staticLights = CalcMeshLightInfluences(mesh, m_staticLights, true);
    auto dynamicLights = CalcMeshLightInfluences(mesh, m_dynamicLights, true);
    Size numLights = AssignLights(
      staticLights,
      dynamicLights,
      lights,
      offset);
    mesh->RenderForward(m_device, eRP_Forward, lights, numLights);
  }

}

void GL4ForwardPipeline::CollectShadowLights(GfxLight* light)
{
  if (!light)
  {
    return;
  }
  iLight* lght = light->GetLight();
  if (!lght->IsCastShadow())
  {
    return;
  }
  switch (lght->GetType())
  {
  case eLT_Point:
  {
    GL4PointLight* pointLight = static_cast<GL4PointLight*>(light->GetLight());
    if (pointLight)
    {
      m_shadowPointLights.push_back(pointLight);
    }
    break;
  }
  case eLT_Directional:
  {
    GL4DirectionalLight* directionalLight = static_cast<GL4DirectionalLight*>(light->GetLight());
    if (directionalLight)
    {
      m_shadowDirectionalLights.push_back(directionalLight);
    }
    break;
  }
  }
}

void GL4ForwardPipeline::SortShadowLights()
{
  std::sort(m_shadowDirectionalLights.begin(), m_shadowDirectionalLights.end(),
    [](GL4DirectionalLight* light0, GL4DirectionalLight* light1) {
      return light0->GetIntensity() > light1->GetIntensity();
    });


  std::sort(m_shadowPointLights.begin(), m_shadowPointLights.end(),
    [](GL4PointLight* light0, GL4PointLight* light1) {
      return light0->GetIntensity() > light1->GetIntensity();
    });

}

void GL4ForwardPipeline::RenderShadowMaps()
{
  SortShadowLights();

  m_pointLightShadowMapAssignment.clear();
  Size i = 0;
  for (auto pointLight : m_shadowPointLights)
  {
    if (i >= MaxLights)
    {
      break;
    }

    GL4RenderTargetCube* shadowMap = GetPointLightShadowMap(i);
    if (!shadowMap)
    {
      return;
    }
    RenderPointShadowMaps(pointLight, shadowMap);
    m_pointLightShadowMapAssignment[pointLight] = shadowMap;

  }
}

GL4RenderTargetCube* GL4ForwardPipeline::GetPointLightShadowMap(Size idx)
{
  if (idx >= MaxLights)
  {
    return nullptr;
  }

  if (m_pointLightShadowMap.size() <= idx)
  {
    iRenderTargetCube::Descriptor desc{};
    desc.Size = 1024;
    GL4RenderTargetCube* cubeMap = static_cast<GL4RenderTargetCube*>(m_device->CreateRenderTarget(desc));

    iTextureCube::Descriptor colorDesc{};
    colorDesc.Size = desc.Size;
    colorDesc.Format = ePF_RGBA;
    colorDesc.MipMaps = false;
    iTextureCube* colorTexture = m_device->CreateTexture(colorDesc);
    colorTexture->SetSampler(GetShadowMapColorSampler());

    iTextureCube::Descriptor depthDesc{};
    depthDesc.Size = desc.Size;
    depthDesc.Format = ePF_Depth;
    depthDesc.MipMaps = false;
    iTextureCube* depthTexture = m_device->CreateTexture(depthDesc);
    depthTexture->SetSampler(GetShadowMapDepthSampler());

    cubeMap->AddColorTexture(colorTexture);
    cubeMap->SetDepthTexture(depthTexture);
    if (!cubeMap->Compile())
    {
      cubeMap->Release();
      colorTexture->Release();
      depthTexture->Release();
      cubeMap = nullptr;
      return nullptr;
    }
    else
    {
      m_pointLightShadowMap.push_back(cubeMap);
    }
  }
  return m_pointLightShadowMap[idx];
}

iSampler* GL4ForwardPipeline::GetShadowMapColorSampler()
{
  if (!m_shadowMapColorSampler)
  {
    m_shadowMapColorSampler = m_device->CreateSampler();
    m_shadowMapColorSampler->SetFilterMode(eFM_MinMagNearest);
    m_shadowMapColorSampler->SetAnisotropy(1);
    m_shadowMapColorSampler->SetAddressU(eTAM_Clamp);
    m_shadowMapColorSampler->SetAddressV(eTAM_Clamp);
    m_shadowMapColorSampler->SetAddressW(eTAM_Clamp);
  }
  return m_shadowMapColorSampler;
}


iSampler* GL4ForwardPipeline::GetShadowMapDepthSampler()
{
  if (!m_shadowMapDepthSampler)
  {
    m_shadowMapDepthSampler = m_device->CreateSampler();
    m_shadowMapDepthSampler->SetFilterMode(eFM_MinMagNearest);
    m_shadowMapDepthSampler->SetAnisotropy(1);
    m_shadowMapDepthSampler->SetAddressU(eTAM_Clamp);
    m_shadowMapDepthSampler->SetAddressV(eTAM_Clamp);
    m_shadowMapDepthSampler->SetAddressW(eTAM_Clamp);
  }
  return m_shadowMapDepthSampler;
}



void GL4ForwardPipeline::RenderPointShadowMaps(GL4PointLight* pointLight, GL4RenderTargetCube* shadowMap)
{
  m_device->SetRenderTarget(shadowMap);
  m_device->SetViewport(0, 0, shadowMap->GetSize(), shadowMap->GetSize());
  m_device->Clear(true, Color4f(0.0f, 0.0f, 0.0f, 1.0f), true, 1.0f, false, 0);


  float near = 1.0f;
  float far = pointLight->GetRange();
  Matrix4f projection;
  m_device->GetPerspectiveProjection(-near, near, -near, near, near, far, projection);
  Matrix4f projections[] = {
    projection,
    projection,
    projection,
    projection,
    projection,
    projection 
  };
  Vector3f pos = pointLight->GetPosition();
  Matrix4f views[6];
  views[0].SetLookAt(pos, pos + Vector3f(1, 0, 0), Vector3f(0, 1, 0));
  views[1].SetLookAt(pos, pos + Vector3f(-1, 0, 0), Vector3f(0, 1, 0));
  views[2].SetLookAt(pos, pos + Vector3f(0, 1, 0), Vector3f(0, 0, -1));
  views[3].SetLookAt(pos, pos + Vector3f(0, -1, 0), Vector3f(0, 0, 1));
  views[4].SetLookAt(pos, pos + Vector3f(0, 0, 1), Vector3f(0, 1, 0));
  views[5].SetLookAt(pos, pos + Vector3f(0, 0, -1), Vector3f(0, 1, 0));


  m_device->SetShadowMapProjectionMatrices(projections, 6);
  m_device->SetShadowMapViewMatrices(views, 6);

  SphereClipper clipper(pos, pointLight->GetRange());


  m_scene->ScanMeshes(nullptr, GfxScene::eSM_Dynamic | GfxScene::eSM_Static,
    [this](GfxMesh* mesh)
    {
      mesh->RenderUnlit(m_device, eRP_ShadowCube);
    }
  );
}


Size GL4ForwardPipeline::AssignLights(
  const std::vector<GfxMesh::Light>& static_lights,
  const std::vector<GfxMesh::Light>& dynamic_lights,
  const GfxLight** lights,
  Size offset)
{
  for (Size s = 0, d = 0, sn = static_lights.size(), dn = dynamic_lights.size();
    offset < MaxLights && (s < sn || d < dn); offset++)
  {
    if (s < sn && d < dn)
    {
      if (static_lights[s].Influence >= dynamic_lights[d].Influence)
      {
        lights[offset] = static_lights[s++].Light;
      }
      else
      {
        lights[offset] = dynamic_lights[d++].Light;
      }
    }
    else if (s < sn)
    {
      lights[offset] = static_lights[s++].Light;
    }
    else if (d < dn)
    {
      lights[offset] = dynamic_lights[d++].Light;
    }
    else
    {
      break;
    }
  }

  return offset;
}


float GL4ForwardPipeline::CalcMeshLightInfluence(const GfxLight* light, const GfxMesh* mesh) const
{
  if (!light || !mesh)
  {
    return 0.0f;
  }

  float halfSize = mesh->GetMesh()->GetBoundingBox().GetDiagonal() / 2.0f;
  // TODO: Take the power of the light from the light ... currently there is no power in the light
  float lightPower = 1.0f;
  float lightDistanceFactor = 0.0f;
  switch (light->GetLight()->GetType())
  {
  case eLT_Directional:
    lightDistanceFactor = 1.0f;
    break;
  case eLT_Point:
    auto pointLight = light->GetLight()->Query<iPointLight>();
    Vector3f lightPos = pointLight->GetPosition();
    Vector3f meshPos = mesh->GetModelMatrix().GetTranslation();
    Vector3f delta = lightPos - meshPos;
    float distance = delta.Length();
    float overlap = pointLight->GetRange() + halfSize - distance;
    if (overlap > 0.0f)
    {
      lightDistanceFactor = overlap / pointLight->GetRange();
    }
    break;
  }
  return lightPower * lightDistanceFactor;
}



std::vector<GfxMesh::Light> GL4ForwardPipeline::CalcMeshLightInfluences(const GfxMesh* mesh, const std::vector<GfxLight*>& lights, bool sorted) const
{
  std::vector<GfxMesh::Light> influences;
  for (GfxLight* light : lights)
  {
    float influence = CalcMeshLightInfluence(light, mesh);
    if (influence <= MinLightInfluence)
    {
      continue;
    }
    GfxMesh::Light l{};
    l.Light = light;
    l.Influence = influence;
    influences.push_back(l);
  }

  if (sorted)
  {
    std::sort(influences.begin(), influences.end(), [](GfxMesh::Light& l0, GfxMesh::Light& l1) { return l0.Influence > l1.Influence; });
  }


  return influences;
}

void GL4ForwardPipeline::AppendLights(GfxMesh* mesh, const std::vector<GfxLight*>& lights) const
{
  if (lights.empty())
  {
    return;
  }

  auto meshLights = CalcMeshLightInfluences(mesh, lights, false);
  for (auto meshLight : meshLights)
  {
    mesh->AddLight(meshLight.Light, meshLight.Influence);
  }
  mesh->SortAndLimitLights(MaxLights);
}

}