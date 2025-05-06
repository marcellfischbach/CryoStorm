
#include <csOpenGL/gl4/pipeline/pssm/csGL4TwinPSSMRenderer.hh>
#include <csOpenGL/gl4/pipeline/csGL4MeshSorter.hh>
#include <csOpenGL/gl4/csGL4Device.hh>
#include <csOpenGL/gl4/csGL4DirectionalLight.hh>
#include <csOpenGL/gl4/csGL4RenderTarget2D.hh>
#include <csOpenGL/gl4/csGL4RenderTarget2DArray.hh>
#include <csOpenGL/gl4/csGL4Texture2DArray.hh>
#include <csCore/csSettings.hh>
#include <csCore/resource/csAssetManager.hh>
#include <csCore/graphics/csCamera.hh>
#include <csCore/graphics/iSampler.hh>
#include <csCore/graphics/csProjector.hh>
#include <csCore/graphics/shading/iShader.hh>
#include <csCore/graphics/shading/iShaderAttribute.hh>
#include <csCore/graphics/scene/iGfxScene.hh>
#include <csCore/graphics/scene/csGfxMesh.hh>
#include <csCore/math/clipper/csCameraClipper.hh>

#include <algorithm>
#include <csCore/csTime.hh>

namespace cs::opengl
{

csGL4TwinPSSMRenderer::csGL4TwinPSSMRenderer()
{
//  for (size_t i = 0; i < m_directionalLightShadowBuffers.ShadowBuffers.size(); i++)
//  {
//    m_directionalLightShadowBuffers.ShadowBuffers[i] = nullptr;
//  }
}

void csGL4TwinPSSMRenderer::Initialize()
{
  const csSettingsFile &settings = csSettings::Get().Graphics();
  m_shadowNear = settings.GetFloat("directional_light.shadow_map.near", 1.0f);
  m_shadowFar  = settings.GetFloat("directional_light.shadow_map.far", 1.0f);
  csVector4f splits = settings.GetVector4f("directional_light.shadow_map.cascades", csVector4f(0.0f, 1.0f, 1.0f, 1.0f));

  m_splits[0] = m_shadowNear + (m_shadowFar - m_shadowNear) * splits.x;
  m_splits[1] = m_shadowNear + (m_shadowFar - m_shadowNear) * splits.y;
  m_splits[2] = m_shadowNear + (m_shadowFar - m_shadowNear) * splits.z;
  m_splits[3] = m_shadowNear + (m_shadowFar - m_shadowNear) * splits.w;


  m_directionalLightShadowBufferSize = settings.GetInt("directional_light.shadow_map.size", 1024) / 2;
  std::string filter = settings.GetText("directional_light.shadow_map.filter.mode", "Plain");
  if (filter == std::string("Plain"))
  {
    m_shadowSamplingMode = ShadowSamplingMode::Plain;
  }
  else if (filter == std::string("PCF"))
  {
    m_shadowSamplingMode = ShadowSamplingMode::PCF;
  }
  else if (filter == std::string("VSM"))
  {
    m_shadowSamplingMode = ShadowSamplingMode::VSM;
  }

  csVector2f distance       = settings.GetVector2f("directional_light.shadow_map.filter.distance", csVector2f(1, 25));
  float      radiusPerCent  = settings.GetFloat("directional_light.shadow_map.filter.radius", 1.0f);
  float      samplesFactor  = settings.GetFloat("directional_light.shadow_map.filter.samplesFactor", 1.0f);
  float      sampleDistance = settings.GetFloat("directional_light.shadow_map.filter.sampleDistance", 0.1f);

  m_shadowMapFilter.Initialize(csVector2f(distance.x, distance.y - distance.x),
                               radiusPerCent / 100.0f,
                               samplesFactor,
                               sampleDistance);


  if (m_shadowSamplingMode == ShadowSamplingMode::VSM)
  {
    m_shadowMappingShader = csAssetManager::Get()->Get<iShader>(
        csAssetLocator("/graphics/gl4/pssm/directional_light_shadow_map_vsm_twin.shader"));
  }
  else
  {
    m_shadowMappingShader = csAssetManager::Get()->Get<iShader>(
        csAssetLocator("/graphics/gl4/pssm/directional_light_shadow_map_twin.shader"));
  }

  if (m_shadowMappingShader)
  {
    m_attrLayersDepth                   = m_shadowMappingShader->GetShaderAttribute("LayersDepth");
    m_attrLayersBias                    = m_shadowMappingShader->GetShaderAttribute("LayersBias");
    m_attrShadowBuffers                 = m_shadowMappingShader->GetShaderAttribute("ShadowBuffers");
    m_attrShadowBufferDatas             = m_shadowMappingShader->GetShaderAttribute("ShadowBufferDatas");
    m_attrDepthBuffer                   = m_shadowMappingShader->GetShaderAttribute("DepthBuffer");
    m_attrShadowMapViewProjectionMatrix = m_shadowMappingShader->GetShaderAttribute("ShadowMapViewProjectionMatrix");
  }

}

void csGL4TwinPSSMRenderer::RenderShadow(const csGL4DirectionalLight *directionalLight,
                                         const cs::csCamera &camera,
                                         const cs::csProjector &projector)
{

  RenderShadowBuffer(directionalLight, camera, projector);
  RenderShadowMap(directionalLight, camera, projector);
//  FilterShadowMap();
}


void csGL4TwinPSSMRenderer::SetDevice(csGL4Device *device)
{
  m_device = device;
}

void csGL4TwinPSSMRenderer::SetDepthBuffer(iTexture2D *depthBuffer)
{
  m_depthBuffer = depthBuffer;

  if (m_depthBuffer)
  {
    m_directionalLightShadowMapWidth  = m_depthBuffer->GetWidth();
    m_directionalLightShadowMapHeight = m_depthBuffer->GetHeight();
  }

}

void csGL4TwinPSSMRenderer::SetScene(iGfxScene *scene)
{
  m_scene = scene;
}

void csGL4TwinPSSMRenderer::SetShadowMap(csGL4RenderTarget2D *shadowMap)
{
  m_directionalLightShadowMap = shadowMap;
}

csGL4RenderTarget2D *csGL4TwinPSSMRenderer::GetShadowMap()
{
  return m_directionalLightShadowMap;
}

void csGL4TwinPSSMRenderer::SetShadowBuffer(iPSSMShadowBufferObject *shadowBufferObject)
{
  m_directionalLightShadowBuffers = static_cast<csGL4TwinPSSMShadowBufferObject *>(shadowBufferObject);
//  m_directionalLightShadowBuffers.ShadowDepth = shadowBufferObject.ShadowDepth;
//  m_directionalLightShadowBuffers.ShadowColor = shadowBufferObject.ShadowColor;
//  for (size_t i = 0; i < m_directionalLightShadowBuffers.ShadowBuffers.size(); ++i)
//  {
//    m_directionalLightShadowBuffers.ShadowBuffers[i] = shadowBufferObject.ShadowBuffers[i];
//  }
}

csGL4TwinPSSMShadowBufferObject *csGL4TwinPSSMRenderer::GetShadowBuffer()
{
  return m_directionalLightShadowBuffers;
}


csGL4RenderTarget2DArray *csGL4TwinPSSMRenderer::GetShadowBuffer(size_t splitLayer)
{
  return m_directionalLightShadowBuffers ? m_directionalLightShadowBuffers->Splits[splitLayer].ShadowBuffers
                                         : nullptr;
}


static void calc_projection_matrix(csGL4Device *device,
                                   csVector3f near[4],
                                   csVector3f far[4],
                                   float n,
                                   float f,
                                   const csMatrix4f &cameraMatrix,
                                   csMatrix4f &projectionMatrix)
{

  float       l = FLT_MAX;
  float       r = -FLT_MAX;
  float       b = FLT_MAX;
  float       t = -FLT_MAX;
  for (size_t i = 0; i < 4; i++)
  {
    csVector3f v = csMatrix4f::Transform(cameraMatrix, near[i]);
    l = ::fminf(v.x, l);
    r = ::fmaxf(v.x, r);
    b = ::fminf(v.y, b);
    t = ::fmaxf(v.y, t);
    v = csMatrix4f::Transform(cameraMatrix, far[i]);
    l = ::fminf(v.x, l);
    r = ::fmaxf(v.x, r);
    b = ::fminf(v.y, b);
    t = ::fmaxf(v.y, t);
  }

  device->GetOrthographicProjection(l, r, b, t, n, f, projectionMatrix);
}


static void calc_projection_matrix_inv(csGL4Device *device,
                                       csVector3f near[4],
                                       csVector3f far[4],
                                       float n,
                                       float f,
                                       const csMatrix4f &cameraMatrix,
                                       csMatrix4f &projectionMatrix)
{

  float       l = FLT_MAX;
  float       r = -FLT_MAX;
  float       b = FLT_MAX;
  float       t = -FLT_MAX;
  for (size_t i = 0; i < 4; i++)
  {
    csVector2f v = csMatrix4f::TransformXY(cameraMatrix, near[i]);
    l = ::fminf(v.x, l);
    r = ::fmaxf(v.x, r);
    b = ::fminf(v.y, b);
    t = ::fmaxf(v.y, t);
    v = csMatrix4f::TransformXY(cameraMatrix, far[i]);
    l = ::fminf(v.x, l);
    r = ::fmaxf(v.x, r);
    b = ::fminf(v.y, b);
    t = ::fmaxf(v.y, t);
  }

  device->GetOrthographicProjectionInv(l, r, b, t, n, f, projectionMatrix);
}


static void calc_center_position(csVector3f near[4], csVector3f far[4], csVector3f &outPos)
{
  outPos        = csVector3f(0.0f, 0.0f, 0.0f);
  for (size_t i = 0; i < 4; i++)
  {
    outPos += near[i];
    outPos += far[i];
  }
  outPos /= 8.0f;
}

void csGL4TwinPSSMRenderer::CalculateSplitPoints(const csProjector &projector,
                                                 const csMatrix4f &camMatInv,
                                                 csVector3f splitPoints[5][4])
{
  projector.GetPoints(m_shadowNear, splitPoints[0]);
  projector.GetPoints(m_splits[0], splitPoints[1]);
  projector.GetPoints(m_splits[1], splitPoints[2]);
  projector.GetPoints(m_splits[2], splitPoints[3]);
  projector.GetPoints(m_splits[3], splitPoints[4]);

  for (size_t layer = 0; layer < 5; layer++)
  {
    for (size_t i = 0; i < 4; i++)
    {
      splitPoints[layer][i] = csMatrix4f::Transform(camMatInv, splitPoints[layer][i]);
    }
  }

}

void csGL4TwinPSSMRenderer::RenderShadowBuffer(const csGL4DirectionalLight *directionalLight,
                                               const cs::csCamera &camera,
                                               const cs::csProjector &projector)
{

  csProjector projectors[2];

  projectors[0].UpdatePerspective(projector.GetLeft(), 0.0f,
                                  projector.GetBottom(), projector.GetTop(),
                                  projector.GetNear(), projector.GetFar());
  projectors[1].UpdatePerspective(0.0f, projector.GetRight(),
                                  projector.GetBottom(), projector.GetTop(),
                                  projector.GetNear(), projector.GetFar());


  const csMatrix4f &camMatInv = camera.GetViewMatrixInv();

  csVector3f fullSplitPoints[5][4];
  csVector3f splitPoints[2][5][4];
  CalculateSplitPoints(projector, camMatInv, fullSplitPoints);
  CalculateSplitPoints(projectors[0], camMatInv, splitPoints[0]);
  CalculateSplitPoints(projectors[1], camMatInv, splitPoints[1]);


  for (size_t splitLayer = 0; splitLayer < 4; splitLayer++)
  {

    csVector3f centerPosition;
    csMatrix4f view, viewInv, proj, projInv;

    calc_center_position(fullSplitPoints[splitLayer], fullSplitPoints[splitLayer + 1], centerPosition);

    view.SetLookAt(centerPosition, centerPosition + directionalLight->GetDirection(), csVector3f(0, 1, 0));
    viewInv.SetLookAtInv(centerPosition, centerPosition + directionalLight->GetDirection(), csVector3f(0, 1, 0));
    calc_projection_matrix_inv(m_device,
                               fullSplitPoints[splitLayer],
                               fullSplitPoints[splitLayer + 1],
                               -1.0f,
                               1.0f,
                               view,
                               projInv);

    csCameraClipper clipper(viewInv, projInv, false, false);
    float           near = FLT_MAX;
    float           far  = -FLT_MAX;
    m_collector.Clear();
    m_scene->ScanMeshes(&clipper, iGfxScene::eSM_Dynamic | iGfxScene::eSM_Static, m_collector);
    std::vector<csGfxMesh *> &meshes = m_collector.GetMeshes(eRenderQueue::Default);

    for (const auto &mesh: meshes)
    {
      if (mesh->IsCastShadow())
      {
        const csVector3f *bboxPoints = mesh->GetBoundingBox().GetPoints();
        for (unsigned    j           = 0; j < 8; j++)
        {
          csVector3f point = bboxPoints[j];
          float      v     = csMatrix4f::TransformZ(view, point);

          near = ::fminf(near, v);
          far  = ::fmaxf(far, v);
        }
      }
    }


    csMatrix4f viewMatrices[2];
    csMatrix4f projMatrices[2];

    //
    // calculate the matrices for the left and right
    for (size_t viewLayer = 0; viewLayer < 2; viewLayer++)
    {
      csVector3f xAxis            = viewInv.GetXAxis().Normalize();
      csVector3f yAxis            = viewInv.GetYAxis().Normalize();
      float      sizeSplit        = GetSplitSize(splitPoints[viewLayer][splitLayer],
                                                 splitPoints[viewLayer][splitLayer + 1]) / 2.0f;
      auto       shadowBufferSize = static_cast<float>(m_directionalLightShadowBufferSize);
      float      modV             = sizeSplit * 2.0f / shadowBufferSize;
      float      onAxisX          = xAxis.Dot(centerPosition);
      float      onAxisY          = yAxis.Dot(centerPosition);
      float      modX             = fmodf(onAxisX, modV);
      float      modY             = fmodf(onAxisY, modV);

      m_device->GetOrthographicProjection(-sizeSplit - modX,
                                          sizeSplit - modX,
                                          -sizeSplit - modY,
                                          sizeSplit - modY,
                                          near,
                                          far,
                                          proj);
      m_device->GetOrthographicProjectionInv(-sizeSplit - modX,
                                             sizeSplit - modX,
                                             -sizeSplit - modY,
                                             sizeSplit - modY,
                                             near,
                                             far,
                                             projInv);

      viewMatrices[viewLayer] = view;
      projMatrices[viewLayer] = proj;
      m_shadowMapViewProjection[viewLayer * 4 + splitLayer] = proj * view;
    }


    m_device->ResetTextures();
    m_device->SetRenderTarget(GetShadowBuffer(splitLayer));
    m_device->SetRenderBuffer(0);
    m_device->SetDepthWrite(true);
    m_device->SetDepthTest(true);
    m_device->SetBlending(false);
    m_device->SetColorWrite(true, true, true, true);
    m_device->Clear(true, csColor4f(1.0f, 1.0f, 1.0f, 1.0f), true, 1.0f, false, 0);

    csMatrix4f ID;
//    m_device->SetProjectionMatrix(ID);
//    m_device->SetViewMatrix(ID);
    m_device->SetShadowMapProjectionMatrices(projMatrices, 2);
    m_device->SetShadowMapViewMatrices(viewMatrices, 2);


    uint64_t        startTime = csTime::GetTime();
    size_t          c         = 0;
    eRenderPass     pass      = eRP_ShadowTwinPSSM;
//    if (m_shadowSamplingMode == ShadowSamplingMode::VSM)
//    {
//      pass = eRP_VSM;
//    }
    for (const auto &mesh: meshes)
    {
      if (mesh->IsCastShadow())
      {
        mesh->RenderUnlit(m_device, pass);
        c++;
      }
    }
  }
}

void csGL4TwinPSSMRenderer::RenderShadowMap(const csGL4DirectionalLight *directionalLight,
                                            const cs::csCamera &camera,
                                            const cs::csProjector &projector)
{
  m_device->ResetTextures();
//  csGL4RenderTarget2D *target = GetDirectionalLightShadowMapTemp();// m_directionalLightShadowMap;
  csGL4RenderTarget2D *target = m_directionalLightShadowMap;
  m_device->SetRenderTarget(target);
  m_device->SetRenderBuffer(0);
  m_device->SetDepthWrite(true);
  m_device->SetDepthTest(false);
  m_device->SetBlending(false);
  m_device->SetColorWrite(true, true, true, true);
  m_device->Clear(true, csColor4f(0.0, 0.0f, 0.0f, 0.0f), true, 1.0f, true, 0);

  m_device->SetShader(m_shadowMappingShader);
  m_device->ResetTextures();
  camera.Bind(m_device);
  projector.Bind(m_device);

  if (m_attrLayersDepth)
  {
    m_attrLayersDepth->Bind(csVector4f(m_splits[0], m_splits[1], m_splits[2], m_splits[3]));
  }
  if (m_attrLayersBias)
  {
    m_attrLayersBias->Bind(directionalLight->GetShadowMapBias());
  }

  if (m_attrShadowBuffers)
  {
    for (size_t i = 0; i < 4; i++)
    {
      eTextureUnit unit = m_device->BindTexture(GetShadowBuffer()->Splits[i].ShadowDepth);
      m_attrShadowBuffers->SetArrayIndex(i);
      m_attrShadowBuffers->Bind(unit);
    }
  }

  if (m_attrShadowBufferDatas && m_shadowSamplingMode == ShadowSamplingMode::VSM)
  {
    for (size_t i = 0; i < 4; i++)
    {
      eTextureUnit unit = m_device->BindTexture(GetShadowBuffer()->Splits[i].ShadowColor);
      m_attrShadowBufferDatas->SetArrayIndex(i);
      m_attrShadowBufferDatas->Bind(unit);
    }
  }
  if (m_attrDepthBuffer)
  {
    eTextureUnit unit = m_device->BindTexture(m_depthBuffer);
    m_attrDepthBuffer->Bind(unit);
  }


  m_device->BindMatrices();
  m_device->BindStandardValues();

  if (m_attrShadowMapViewProjectionMatrix)
  {
    m_attrShadowMapViewProjectionMatrix->SetArrayIndex(0);
    m_attrShadowMapViewProjectionMatrix->Bind(m_shadowMapViewProjection.data(), 8);
  }

  m_device->RenderFullscreen();
}

void csGL4TwinPSSMRenderer::FilterShadowMap()
{
  m_device->ResetTextures();

  m_shadowMapFilter.Render(m_device,
                           m_depthBuffer,
                           GetDirectionalLightShadowMapTemp()->GetColorTexture(0),
                           m_directionalLightShadowMap
  );

}


csGL4TwinPSSMShadowBufferObject *csGL4TwinPSSMRenderer::CreateDirectionalLightShadowBuffer()
{
  csGL4TwinPSSMShadowBufferObject *sbo = new csGL4TwinPSSMShadowBufferObject();

  for (size_t viewLayer = 0; viewLayer < 4; viewLayer++)
  {
    if (m_shadowSamplingMode == ShadowSamplingMode::VSM)
    {
      iTexture2DArray::Descriptor colorDesc {};
      colorDesc.Width                    = m_directionalLightShadowBufferSize;
      colorDesc.Height                   = m_directionalLightShadowBufferSize;
      colorDesc.Layers                   = 2;
      colorDesc.Format                   = ePF_RG32F;
      colorDesc.MipMaps                  = false;
      sbo->Splits[viewLayer].ShadowColor = m_device->CreateTexture(colorDesc).Query<csGL4Texture2DArray>();
      sbo->Splits[viewLayer].ShadowColor->SetSampler(GetShadowBufferColorSampler());
    }

    iTexture2DArray::Descriptor depthDesc {};
    depthDesc.Width                    = m_directionalLightShadowBufferSize;
    depthDesc.Height                   = m_directionalLightShadowBufferSize;
    depthDesc.Layers                   = 2;
    depthDesc.Format                   = ePF_Depth;
    depthDesc.MipMaps                  = false;
    sbo->Splits[viewLayer].ShadowDepth = m_device->CreateTexture(depthDesc).Query<csGL4Texture2DArray>();
    sbo->Splits[viewLayer].ShadowDepth->SetSampler(GetShadowBufferDepthSampler());


    iRenderTarget2DArray::Descriptor desc {};
    desc.Width  = (uint16_t) m_directionalLightShadowBufferSize;
    desc.Height = (uint16_t) m_directionalLightShadowBufferSize;
    desc.Layer  = 2;
    auto shadowRenderTarget = m_device->CreateRenderTarget(desc).Query<csGL4RenderTarget2DArray>();


    if (sbo->Splits[viewLayer].ShadowColor)
    {
      shadowRenderTarget->AddColorTexture(sbo->Splits[viewLayer].ShadowColor);
    }
    if (sbo->Splits[viewLayer].ShadowDepth)
    {
      shadowRenderTarget->SetDepthTexture(sbo->Splits[viewLayer].ShadowDepth);
    }

    if (!shadowRenderTarget->Compile())
    {
      printf("Unable to compile\n");
      shadowRenderTarget = nullptr;
    }

    sbo->Splits[viewLayer].ShadowBuffers = shadowRenderTarget;
  }
  return sbo;

}


bool csGL4TwinPSSMRenderer::IsShadowMapValid(csGL4RenderTarget2D *shadowMap) const
{
  return shadowMap
         && shadowMap->GetWidth() == m_directionalLightShadowMapWidth
         && shadowMap->GetHeight() == m_directionalLightShadowMapHeight;
}

bool csGL4TwinPSSMRenderer::IsShadowBufferValid(iPSSMShadowBufferObject *shadowMap) const
{
  if (!shadowMap || shadowMap->m_type != 1)
  {
    return false;
  }
  csGL4RenderTarget2DArray
      *buffer = reinterpret_cast<csGL4TwinPSSMShadowBufferObject *>(shadowMap)->Splits[0].ShadowBuffers;
  return buffer
         && buffer->GetWidth() == m_directionalLightShadowBufferSize
         && buffer->GetHeight() == m_directionalLightShadowBufferSize;
}

csGL4RenderTarget2D *csGL4TwinPSSMRenderer::GetDirectionalLightShadowMapTemp()
{
  csGL4RenderTarget2D *target = m_directionalLightShadowMapTemp;
  if (m_directionalLightShadowMapTemp)
  {
    if (m_directionalLightShadowMapWidth == m_directionalLightShadowMapTemp->GetWidth()
        && m_directionalLightShadowMapHeight == m_directionalLightShadowMapTemp->GetHeight())
    {
      return m_directionalLightShadowMapTemp;
    }
  }

  m_directionalLightShadowMapTemp = CreateDirectionalLightShadowMap();
  return m_directionalLightShadowMapTemp;
}


csOwned<csGL4RenderTarget2D> csGL4TwinPSSMRenderer::CreateDirectionalLightShadowMap()
{
  iRenderTarget2D::Descriptor desc {};
  desc.Width  = (uint16_t) m_directionalLightShadowMapWidth;
  desc.Height = (uint16_t) m_directionalLightShadowMapHeight;
  auto target = m_device->CreateRenderTarget(desc).Query<csGL4RenderTarget2D>();

  iTexture2D::Descriptor colorDesc {};
  colorDesc.Width   = (uint16_t) m_directionalLightShadowMapWidth;
  colorDesc.Height  = (uint16_t) m_directionalLightShadowMapHeight;
  colorDesc.Format  = ePF_RGB;
  colorDesc.MipMaps = false;
  auto colorTexture = m_device->CreateTexture(colorDesc);

  colorTexture->SetSampler(GetShadowMapColorSampler());
  target->AddColorTexture(colorTexture.Data());
  target->SetDepthBuffer(ePF_Depth);

  if (!target->Compile())
  {
    target = nullptr;
  }

  return target;
}


float csGL4TwinPSSMRenderer::GetSplitSize(const csVector3f *near, const csVector3f *far)
{
  /*
  *     1         3
  *      +-------+
  *      |       |
  *      |       |
  *      |       |
  *      +-------+
  *     0         2
  */
  float dotFar03     = (far[0] - far[3]).Dot();
  float dotFar21     = (far[2] - far[1]).Dot();
  float dotNearFar03 = (near[0] - far[3]).Dot();
  float dotNearFar21 = (near[2] - far[1]).Dot();

  //  float dotFar03 = Vector3f::Sub(far[0], far[3], tmp).Dot();
  //  float dotFar21 = Vector3f::Sub(far[2], far[1], tmp).Dot();
  //  float dotNearFar03 = Vector3f::Sub(near[0], far[3], tmp).Dot();
  //  float dotNearFar21 = Vector3f::Sub(near[2], far[1], tmp).Dot();
  float maxDist = ceMax(ceMax(dotFar03, dotFar21), ceMax(dotNearFar03, dotNearFar21));

  // add a 10% extra
  return ceSqrt(maxDist) * 1.1f;
}

iSampler *csGL4TwinPSSMRenderer::GetShadowMapColorSampler()
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

iSampler *csGL4TwinPSSMRenderer::GetShadowBufferColorSampler()
{
  if (!m_shadowBufferColorSampler)
  {
    m_shadowBufferColorSampler = m_device->CreateSampler();
    m_shadowBufferColorSampler->SetFilterMode(eFM_MinMagLinear);
    m_shadowBufferColorSampler->SetAnisotropy(1);
    m_shadowBufferColorSampler->SetAddressU(eTAM_Clamp);
    m_shadowBufferColorSampler->SetAddressV(eTAM_Clamp);
    m_shadowBufferColorSampler->SetAddressW(eTAM_Clamp);
  }
  return m_shadowBufferColorSampler;
}

iSampler *csGL4TwinPSSMRenderer::GetShadowBufferDepthSampler()
{
  if (!m_shadowMapDepthSampler)
  {
    m_shadowMapDepthSampler = m_device->CreateSampler();
    if (m_shadowSamplingMode == ShadowSamplingMode::PCF)
    {
      m_shadowMapDepthSampler->SetFilterMode(eFM_MinMagLinear);
    }
    else
    {
      m_shadowMapDepthSampler->SetFilterMode(eFM_MinMagNearest);
    }
    m_shadowMapDepthSampler->SetAnisotropy(1);
    m_shadowMapDepthSampler->SetAddressU(eTAM_Clamp);
    m_shadowMapDepthSampler->SetAddressV(eTAM_Clamp);
    m_shadowMapDepthSampler->SetAddressW(eTAM_Clamp);
    m_shadowMapDepthSampler->SetTextureCompareMode(eTCM_CompareToR);
    m_shadowMapDepthSampler->SetTextureCompareFunc(eCF_LessOrEqual);
  }
  return m_shadowMapDepthSampler;
}

csGL4TwinPSSMShadowBufferObject::csGL4TwinPSSMShadowBufferObject()
{
  m_type = 1;
  for (size_t i = 0; i < 4; i++)
  {
    Splits[i].ShadowDepth = nullptr;
    Splits[i].ShadowColor = nullptr;
    Splits[i].ShadowBuffers = nullptr;
  }
}


void csGL4TwinPSSMShadowBufferObject::DeleteSelf()
{
  delete this;
}


}
