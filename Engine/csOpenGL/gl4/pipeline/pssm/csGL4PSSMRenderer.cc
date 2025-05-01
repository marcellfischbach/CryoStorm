
#include <csOpenGL/gl4/pipeline/pssm/csGL4PSSMRenderer.hh>
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

csGL4PSSMRenderer::csGL4PSSMRenderer()
{
//  for (size_t i = 0; i < m_directionalLightShadowBuffers.ShadowBuffers.size(); i++)
//  {
//    m_directionalLightShadowBuffers.ShadowBuffers[i] = nullptr;
//  }
}

void csGL4PSSMRenderer::Initialize()
{
  const csSettingsFile &settings = csSettings::Get().Graphics();
  m_shadowNear = settings.GetFloat("directional_light.shadow_map.near", 1.0f);
  m_shadowFar  = settings.GetFloat("directional_light.shadow_map.far", 1.0f);
  csVector4f splits = settings.GetVector4f("directional_light.shadow_map.cascades", csVector4f(0.0f, 1.0f, 1.0f, 1.0f));

  m_splits[0] = m_shadowNear + (m_shadowFar - m_shadowNear) * splits.x;
  m_splits[1] = m_shadowNear + (m_shadowFar - m_shadowNear) * splits.y;
  m_splits[2] = m_shadowNear + (m_shadowFar - m_shadowNear) * splits.z;
  m_splits[3] = m_shadowNear + (m_shadowFar - m_shadowNear) * splits.w;


  m_directionalLightShadowBufferSize = settings.GetInt("directional_light.shadow_map.size", 1024);
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
        csAssetLocator("/graphics/gl4/pssm/directional_light_shadow_map_vsm.shader"));
  }
  else
  {
    m_shadowMappingShader = csAssetManager::Get()->Get<iShader>(
        csAssetLocator("/graphics/gl4/pssm/directional_light_shadow_map.shader"));
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


void csGL4PSSMRenderer::SetDevice(csGL4Device *device)
{
  m_device = device;
}

void csGL4PSSMRenderer::SetDepthBuffer(iTexture2D *depthBuffer)
{
  m_depthBuffer = depthBuffer;

  if (m_depthBuffer)
  {
    m_directionalLightShadowMapWidth  = m_depthBuffer->GetWidth();
    m_directionalLightShadowMapHeight = m_depthBuffer->GetHeight();
  }

}

void csGL4PSSMRenderer::SetScene(iGfxScene *scene)
{
  m_scene = scene;
}

void csGL4PSSMRenderer::SetShadowMap(csGL4RenderTarget2D *shadowMap)
{
  m_directionalLightShadowMap = shadowMap;
}

csGL4RenderTarget2D *csGL4PSSMRenderer::GetShadowMap()
{
  return m_directionalLightShadowMap;
}

void csGL4PSSMRenderer::SetShadowBuffer(iPSSMShadowBufferObject *shadowBufferObject)
{
  m_directionalLightShadowBuffers = static_cast<csGL4PSSMShadowBufferObject *>(shadowBufferObject);
//  m_directionalLightShadowBuffers.ShadowDepth = shadowBufferObject.ShadowDepth;
//  m_directionalLightShadowBuffers.ShadowColor = shadowBufferObject.ShadowColor;
//  for (size_t i = 0; i < m_directionalLightShadowBuffers.ShadowBuffers.size(); ++i)
//  {
//    m_directionalLightShadowBuffers.ShadowBuffers[i] = shadowBufferObject.ShadowBuffers[i];
//  }
}

csGL4PSSMShadowBufferObject *csGL4PSSMRenderer::GetShadowBuffer()
{
  return m_directionalLightShadowBuffers;
}

csGL4RenderTarget2D *csGL4PSSMRenderer::GetShadowBuffer(size_t splitLayer)
{
  return m_directionalLightShadowBuffers ? m_directionalLightShadowBuffers->ShadowBuffers[splitLayer] : nullptr;
}


void csGL4PSSMRenderer::RenderShadow(const csGL4DirectionalLight *directionalLight,
                                     const cs::csCamera &camera,
                                     const cs::csProjector &projector)
{

  RenderShadowBuffer(directionalLight, camera, projector);
  RenderShadowMap(directionalLight, camera, projector);
//  FilterShadowMap();
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


void csGL4PSSMRenderer::RenderShadowBuffer(const csGL4DirectionalLight *directionalLight,
                                           const cs::csCamera &camera,
                                           const cs::csProjector &projector)
{

  csVector3f splitPoints[5][4];
  projector.GetPoints(m_shadowNear, splitPoints[0]);
  projector.GetPoints(m_splits[0], splitPoints[1]);
  projector.GetPoints(m_splits[1], splitPoints[2]);
  projector.GetPoints(m_splits[2], splitPoints[3]);
  projector.GetPoints(m_splits[3], splitPoints[4]);

  const csMatrix4f &camMatInv = camera.GetViewMatrixInv();


  for (size_t i = 0; i < 4; i++)
  {
    splitPoints[0][i] = csMatrix4f::Transform(camMatInv, splitPoints[0][i]);
    splitPoints[1][i] = csMatrix4f::Transform(camMatInv, splitPoints[1][i]);
    splitPoints[2][i] = csMatrix4f::Transform(camMatInv, splitPoints[2][i]);
    splitPoints[3][i] = csMatrix4f::Transform(camMatInv, splitPoints[3][i]);
    splitPoints[4][i] = csMatrix4f::Transform(camMatInv, splitPoints[4][i]);
  }

  size_t      maxShadowCaster = 0;
  for (size_t i               = 0; i < 4; i++)
  {


    csVector3f centerPosition;
    csMatrix4f view, viewInv, proj, projInv;

    calc_center_position(splitPoints[i], splitPoints[i + 1], centerPosition);

    view.SetLookAt(centerPosition, centerPosition + directionalLight->GetDirection(), csVector3f(0, 1, 0));
    viewInv.SetLookAtInv(centerPosition, centerPosition + directionalLight->GetDirection(), csVector3f(0, 1, 0));

//    printf ("%.2f %.2f %.2f -> %.2f %.2f %.2f\n",
//            centerPosition.x,
//            centerPosition.y,
//            centerPosition.z,
//            directionalLight->GetDirection().x,
//            directionalLight->GetDirection().y,
//            directionalLight->GetDirection().z
//            );
//    fflush(stdout);

    csVector3f xAxis            = viewInv.GetXAxis().Normalize();
    csVector3f yAxis            = viewInv.GetYAxis().Normalize();
    float      sizeSplit        = GetSplitSize(splitPoints[i], splitPoints[i + 1]) / 2.0f;
    auto       shadowBufferSize = static_cast<float>(m_directionalLightShadowBufferSize);
    float      modV             = sizeSplit * 2.0f / shadowBufferSize;
    float      onAxisX          = xAxis.Dot(centerPosition);
    float      onAxisY          = yAxis.Dot(centerPosition);
    float      modX             = fmodf(onAxisX, modV);
    float      modY             = fmodf(onAxisY, modV);


    calc_projection_matrix_inv(m_device, splitPoints[i], splitPoints[i + 1], -1.0f, 1.0f, view, projInv);


    csCameraClipper clipper(viewInv, projInv, false, false);


    float near = FLT_MAX;
    float far  = -FLT_MAX;
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

    maxShadowCaster = std::max(maxShadowCaster, meshes.size());


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

//    calc_projection_matrix(m_device, splitPoints[i], splitPoints[i + 1], near, far, view, proj);


    m_shadowMapViewProjection[i] = proj * view;

    m_device->ResetTextures();
    m_device->SetRenderTarget(GetShadowBuffer(i));
    m_device->SetRenderBuffer(0);
    m_device->SetDepthWrite(true);
    m_device->SetDepthTest(true);
    m_device->SetBlending(false);
    m_device->SetColorWrite(true, true, true, true);
    m_device->Clear(true, csColor4f(1.0f, 1.0f, 1.0f, 1.0f), true, 1.0f, false, 0);

    m_device->SetProjectionMatrix(proj);
    m_device->SetViewMatrix(view);


    uint64_t    startTime = csTime::GetTime();
    size_t      c         = 0;
    eRenderPass pass      = eRP_Depth;
    if (m_shadowSamplingMode == ShadowSamplingMode::VSM)
    {
      pass = eRP_VSM;
    }
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


void csGL4PSSMRenderer::RenderShadowMap(const csGL4DirectionalLight *directionalLight,
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
//    for (size_t i = 0; i < 4; i++)
    eTextureUnit unit = m_device->BindTexture(GetShadowBuffer()->ShadowDepth);
    m_attrShadowBuffers->SetArrayIndex(0);
    m_attrShadowBuffers->Bind(unit);
  }
  if (m_attrShadowBufferDatas && m_shadowSamplingMode == ShadowSamplingMode::VSM)
  {
    eTextureUnit unit = m_device->BindTexture(GetShadowBuffer()->ShadowColor);
    m_attrShadowBufferDatas->SetArrayIndex(0);
    m_attrShadowBufferDatas->Bind(unit);
  }
  if (m_attrDepthBuffer)
  {
    eTextureUnit unit = m_device->BindTexture(m_depthBuffer);
    m_attrDepthBuffer->Bind(unit);
  }
  if (m_attrShadowMapViewProjectionMatrix)
  {
    m_attrShadowMapViewProjectionMatrix->SetArrayIndex(0);
    m_attrShadowMapViewProjectionMatrix->Bind(m_shadowMapViewProjection.data(), 4);
  }

  m_device->BindMatrices();
  m_device->BindStandardValues();

  m_device->RenderFullscreen();
}

void csGL4PSSMRenderer::FilterShadowMap()
{
  m_device->ResetTextures();

  m_shadowMapFilter.Render(m_device,
                           m_depthBuffer,
                           GetDirectionalLightShadowMapTemp()->GetColorTexture(0),
                           m_directionalLightShadowMap
  );

}


csGL4PSSMShadowBufferObject *csGL4PSSMRenderer::CreateDirectionalLightShadowBuffer()
{
  csGL4PSSMShadowBufferObject *sbo = new csGL4PSSMShadowBufferObject();

  if (m_shadowSamplingMode == ShadowSamplingMode::VSM)
  {
    iTexture2DArray::Descriptor colorDesc {};
    colorDesc.Width   = m_directionalLightShadowBufferSize;
    colorDesc.Height  = m_directionalLightShadowBufferSize;
    colorDesc.Layers  = 4;
    colorDesc.Format  = ePF_RG32F;
    colorDesc.MipMaps = false;
    sbo->ShadowColor  = m_device->CreateTexture(colorDesc).Query<csGL4Texture2DArray>();
    sbo->ShadowColor->SetSampler(GetShadowBufferColorSampler());
  }

  iTexture2DArray::Descriptor depthDesc {};
  depthDesc.Width   = m_directionalLightShadowBufferSize;
  depthDesc.Height  = m_directionalLightShadowBufferSize;
  depthDesc.Layers  = 4;
  depthDesc.Format  = ePF_Depth;
  depthDesc.MipMaps = false;
  sbo->ShadowDepth  = m_device->CreateTexture(depthDesc).Query<csGL4Texture2DArray>();
  sbo->ShadowDepth->SetSampler(GetShadowBufferDepthSampler());


  for (size_t i = 0, in = sbo->ShadowBuffers.size(); i < in; i++)
  {
    iRenderTarget2D::Descriptor desc {};
    desc.Width  = (uint16_t) m_directionalLightShadowBufferSize;
    desc.Height = (uint16_t) m_directionalLightShadowBufferSize;
    auto shadowRenderTarget = m_device->CreateRenderTarget(desc).Query<csGL4RenderTarget2D>();


    if (sbo->ShadowColor)
    {
      shadowRenderTarget->AddColorTexture(sbo->ShadowColor, i);
    }
    if (sbo->ShadowDepth)
    {
      shadowRenderTarget->SetDepthTexture(sbo->ShadowDepth, i);
    }

    if (!shadowRenderTarget->Compile())
    {
      printf("Unable to compile\n");
      shadowRenderTarget = nullptr;
    }

    sbo->ShadowBuffers[i] = shadowRenderTarget;
  }
  return sbo;

}

void csGL4PSSMRenderer::DeleteDirectionalLightShadowBuffer(cs::opengl::iPSSMShadowBufferObject *sbo)
{
  if (!sbo)
  {
    return;
  }
  csGL4PSSMShadowBufferObject *shadowBuffer = reinterpret_cast<csGL4PSSMShadowBufferObject *>(sbo);
  delete shadowBuffer;
}

bool csGL4PSSMRenderer::IsShadowMapValid(csGL4RenderTarget2D *shadowMap) const
{
  return shadowMap
         && shadowMap->GetWidth() == m_directionalLightShadowMapWidth
         && shadowMap->GetHeight() == m_directionalLightShadowMapHeight;
}

bool csGL4PSSMRenderer::IsShadowBufferValid(iPSSMShadowBufferObject *shadowMap) const
{
  if (!shadowMap)
  {
    return false;
  }
  csGL4RenderTarget2D *buffer = reinterpret_cast<csGL4PSSMShadowBufferObject *>(shadowMap)->ShadowBuffers[0];
  return buffer
         && buffer->GetWidth() == m_directionalLightShadowBufferSize
         && buffer->GetHeight() == m_directionalLightShadowBufferSize;
}

csGL4RenderTarget2D *csGL4PSSMRenderer::GetDirectionalLightShadowMapTemp()
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


csOwned<csGL4RenderTarget2D> csGL4PSSMRenderer::CreateDirectionalLightShadowMap()
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


float csGL4PSSMRenderer::GetSplitSize(const csVector3f *near, const csVector3f *far)
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

iSampler *csGL4PSSMRenderer::GetShadowMapColorSampler()
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

iSampler *csGL4PSSMRenderer::GetShadowBufferColorSampler()
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

iSampler *csGL4PSSMRenderer::GetShadowBufferDepthSampler()
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

csGL4PSSMShadowBufferObject::csGL4PSSMShadowBufferObject()
{
  ShadowDepth = nullptr;
  ShadowColor = nullptr;
  ShadowBuffers[0] = nullptr;
  ShadowBuffers[1] = nullptr;
  ShadowBuffers[2] = nullptr;
  ShadowBuffers[3] = nullptr;
}

csGL4PSSMShadowBufferObject::csGL4PSSMShadowBufferObject(const csGL4PSSMShadowBufferObject &sbo)
{
  ShadowDepth = sbo.ShadowDepth;
  ShadowColor = sbo.ShadowColor;
  ShadowBuffers[0] = sbo.ShadowBuffers[0];
  ShadowBuffers[1] = sbo.ShadowBuffers[1];
  ShadowBuffers[2] = sbo.ShadowBuffers[2];
  ShadowBuffers[3] = sbo.ShadowBuffers[3];
}

csGL4PSSMShadowBufferObject::csGL4PSSMShadowBufferObject(csGL4PSSMShadowBufferObject &&sbo)
{
  ShadowDepth = sbo.ShadowDepth;
  ShadowColor = sbo.ShadowColor;
  ShadowBuffers[0] = sbo.ShadowBuffers[0];
  ShadowBuffers[1] = sbo.ShadowBuffers[1];
  ShadowBuffers[2] = sbo.ShadowBuffers[2];
  ShadowBuffers[3] = sbo.ShadowBuffers[3];
  sbo.ShadowDepth = nullptr;
  sbo.ShadowColor = nullptr;
  sbo.ShadowBuffers[0] = nullptr;
  sbo.ShadowBuffers[1] = nullptr;
  sbo.ShadowBuffers[2] = nullptr;
  sbo.ShadowBuffers[3] = nullptr;
}

csGL4PSSMShadowBufferObject::~csGL4PSSMShadowBufferObject()
{

}

csGL4PSSMShadowBufferObject &csGL4PSSMShadowBufferObject::operator=(const csGL4PSSMShadowBufferObject &sbo)
{
  ShadowDepth = sbo.ShadowDepth;
  ShadowColor = sbo.ShadowColor;
  ShadowBuffers[0] = sbo.ShadowBuffers[0];
  ShadowBuffers[1] = sbo.ShadowBuffers[1];
  ShadowBuffers[2] = sbo.ShadowBuffers[2];
  ShadowBuffers[3] = sbo.ShadowBuffers[3];
  return *this;
}


}
