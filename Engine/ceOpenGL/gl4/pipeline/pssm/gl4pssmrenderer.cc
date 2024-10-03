
#include <ceOpenGL/gl4/pipeline/pssm/gl4pssmrenderer.hh>
#include <ceOpenGL/gl4/pipeline/gl4meshsorter.hh>
#include <ceOpenGL/gl4/gl4device.hh>
#include <ceOpenGL/gl4/gl4directionallight.hh>
#include <ceOpenGL/gl4/gl4rendertarget2d.hh>
#include <ceOpenGL/gl4/gl4rendertarget2darray.hh>
#include <ceOpenGL/gl4/gl4texture2darray.hh>
#include <ceCore/settings.hh>
#include <ceCore/resource/assetmanager.hh>
#include <ceCore/graphics/camera.hh>
#include <ceCore/graphics/isampler.hh>
#include <ceCore/graphics/projector.hh>
#include <ceCore/graphics/shading/ishader.hh>
#include <ceCore/graphics/shading/ishaderattribute.hh>
#include <ceCore/graphics/scene/igfxscene.hh>
#include <ceCore/graphics/scene/gfxmesh.hh>
#include <ceCore/math/clipper/cameraclipper.hh>

#include <algorithm>
#include <ceCore/time.hh>

namespace ce::opengl
{

GL4PSSMRenderer::GL4PSSMRenderer()
{
  for (size_t i = 0; i < m_directionalLightShadowBuffers.ShadowBuffers.size(); i++)
  {
    m_directionalLightShadowBuffers.ShadowBuffers[i] = nullptr;
  }
}

void GL4PSSMRenderer::Initialize()
{
  const SettingsFile &settings = Settings::Get().Graphics();
  m_shadowNear = settings.GetFloat("directional_light.shadow_map.near", 1.0f);
  m_shadowFar  = settings.GetFloat("directional_light.shadow_map.far", 1.0f);
  Vector4f splits = settings.GetVector4f("directional_light.shadow_map.cascades", Vector4f(0.0f, 1.0f, 1.0f, 1.0f));

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

  Vector2f distance       = settings.GetVector2f("directional_light.shadow_map.filter.distance", Vector2f(1, 25));
  float    radiusPerCent  = settings.GetFloat("directional_light.shadow_map.filter.radius", 1.0f);
  float    samplesFactor  = settings.GetFloat("directional_light.shadow_map.filter.samplesFactor", 1.0f);
  float    sampleDistance = settings.GetFloat("directional_light.shadow_map.filter.sampleDistance", 0.1f);

  m_shadowMapFilter.Initialize(Vector2f(distance.x, distance.y - distance.x),
                               radiusPerCent / 100.0f,
                               samplesFactor,
                               sampleDistance);


  m_shadowMappingShader = AssetManager::Get()->Get<iShader>(
      ResourceLocator("file://${engine}/opengl/gl4/pssm/directional_light_shadow_map.shader"));
  if (m_shadowMappingShader)
  {
    m_attrLayersDepth   = m_shadowMappingShader->GetShaderAttribute("LayersDepth");
    m_attrLayersBias    = m_shadowMappingShader->GetShaderAttribute("LayersBias");
    m_attrShadowBuffers = m_shadowMappingShader->GetShaderAttribute("ShadowBuffers");
    m_attrDepthBuffer   = m_shadowMappingShader->GetShaderAttribute("DepthBuffer");
  }

}


void GL4PSSMRenderer::SetDevice(GL4Device *device)
{
  CS_SET(m_device, device);
}

void GL4PSSMRenderer::SetDepthBuffer(iTexture2D *depthBuffer)
{
  CS_SET(m_depthBuffer, depthBuffer);

  if (m_depthBuffer)
  {
    m_directionalLightShadowMapWidth  = m_depthBuffer->GetWidth();
    m_directionalLightShadowMapHeight = m_depthBuffer->GetHeight();
  }

}

void GL4PSSMRenderer::SetScene(iGfxScene *scene)
{
  CS_SET(m_scene, scene);
}

void GL4PSSMRenderer::SetShadowMap(GL4RenderTarget2D *shadowMap)
{
  CS_SET(m_directionalLightShadowMap, shadowMap);
}

GL4RenderTarget2D *GL4PSSMRenderer::GetShadowMap()
{
  return m_directionalLightShadowMap;
}

void GL4PSSMRenderer::SetShadowBuffer(const GL4PSSMShadowBufferObject &shadowBufferObject)
{
  m_directionalLightShadowBuffers.ShadowDepth = shadowBufferObject.ShadowDepth;
  m_directionalLightShadowBuffers.ShadowColor = shadowBufferObject.ShadowColor;
  for (size_t i = 0; i < m_directionalLightShadowBuffers.ShadowBuffers.size(); ++i)
  {
    CS_SET(m_directionalLightShadowBuffers.ShadowBuffers[i], shadowBufferObject.ShadowBuffers[i]);
  }
}

const GL4PSSMShadowBufferObject &GL4PSSMRenderer::GetShadowBuffer()
{
  return m_directionalLightShadowBuffers;
}

GL4RenderTarget2D *GL4PSSMRenderer::GetShadowBuffer(size_t splitLayer)
{
  return m_directionalLightShadowBuffers.ShadowBuffers[splitLayer];
}


void GL4PSSMRenderer::RenderShadow(const GL4DirectionalLight *directionalLight,
                                   const ce::Camera &camera,
                                   const ce::Projector &projector)
{

  RenderShadowBuffer(directionalLight, camera, projector);
  RenderShadowMap(directionalLight, camera, projector);
  FilterShadowMap();
}

static void calc_projection_matrix(GL4Device *device,
                                   Vector3f near[4],
                                   Vector3f far[4],
                                   float n,
                                   float f,
                                   const Matrix4f &cameraMatrix,
                                   Matrix4f &projectionMatrix)
{

  float       l = FLT_MAX;
  float       r = -FLT_MAX;
  float       b = FLT_MAX;
  float       t = -FLT_MAX;
  for (size_t i = 0; i < 4; i++)
  {
    Vector3f v = Matrix4f::Transform(cameraMatrix, near[i]);
    l = ::fminf(v.x, l);
    r = ::fmaxf(v.x, r);
    b = ::fminf(v.y, b);
    t = ::fmaxf(v.y, t);
    v = Matrix4f::Transform(cameraMatrix, far[i]);
    l = ::fminf(v.x, l);
    r = ::fmaxf(v.x, r);
    b = ::fminf(v.y, b);
    t = ::fmaxf(v.y, t);
  }

  device->GetOrthographicProjection(l, r, b, t, n, f, projectionMatrix);
}


static void calc_projection_matrix_inv(GL4Device *device,
                                       Vector3f near[4],
                                       Vector3f far[4],
                                       float n,
                                       float f,
                                       const Matrix4f &cameraMatrix,
                                       Matrix4f &projectionMatrix)
{

  float       l = FLT_MAX;
  float       r = -FLT_MAX;
  float       b = FLT_MAX;
  float       t = -FLT_MAX;
  for (size_t i = 0; i < 4; i++)
  {
    Vector2f v = Matrix4f::TransformXY(cameraMatrix, near[i]);
    l = ::fminf(v.x, l);
    r = ::fmaxf(v.x, r);
    b = ::fminf(v.y, b);
    t = ::fmaxf(v.y, t);
    v = Matrix4f::TransformXY(cameraMatrix, far[i]);
    l = ::fminf(v.x, l);
    r = ::fmaxf(v.x, r);
    b = ::fminf(v.y, b);
    t = ::fmaxf(v.y, t);
  }

  device->GetOrthographicProjectionInv(l, r, b, t, n, f, projectionMatrix);
}


static void calc_center_position(Vector3f near[4], Vector3f far[4], Vector3f &outPos)
{
  outPos        = Vector3f(0.0f, 0.0f, 0.0f);
  for (size_t i = 0; i < 4; i++)
  {
    outPos += near[i];
    outPos += far[i];
  }
  outPos /= 8.0f;
}


void GL4PSSMRenderer::RenderShadowBuffer(const GL4DirectionalLight *directionalLight,
                                         const ce::Camera &camera,
                                         const ce::Projector &projector)
{

  Vector3f splitPoints[5][4];
  projector.GetPoints(m_shadowNear, splitPoints[0]);
  projector.GetPoints(m_splits[0], splitPoints[1]);
  projector.GetPoints(m_splits[1], splitPoints[2]);
  projector.GetPoints(m_splits[2], splitPoints[3]);
  projector.GetPoints(m_splits[3], splitPoints[4]);

  const Matrix4f &camMatInv = camera.GetViewMatrixInv();

  Matrix4f shadowMapView[4];
  Matrix4f shadowMapProjection[4];

  for (size_t i = 0; i < 4; i++)
  {
    splitPoints[0][i] = Matrix4f::Transform(camMatInv, splitPoints[0][i]);
    splitPoints[1][i] = Matrix4f::Transform(camMatInv, splitPoints[1][i]);
    splitPoints[2][i] = Matrix4f::Transform(camMatInv, splitPoints[2][i]);
    splitPoints[3][i] = Matrix4f::Transform(camMatInv, splitPoints[3][i]);
    splitPoints[4][i] = Matrix4f::Transform(camMatInv, splitPoints[4][i]);
  }

  size_t      maxShadowCaster = 0;
  for (size_t i               = 0; i < 4; i++)
  {


    Vector3f centerPosition;
    Matrix4f view, viewInv, proj, projInv;

    calc_center_position(splitPoints[i], splitPoints[i + 1], centerPosition);

    view.SetLookAt(centerPosition, centerPosition + directionalLight->GetDirection(), Vector3f(0, 1, 0));
    viewInv.SetLookAtInv(centerPosition, centerPosition + directionalLight->GetDirection(), Vector3f(0, 1, 0));


    Vector3f xAxis            = viewInv.GetXAxis().Normalize();
    Vector3f yAxis            = viewInv.GetYAxis().Normalize();
    float    sizeSplit        = GetSplitSize(splitPoints[i], splitPoints[i + 1]) / 2.0f;
    auto     shadowBufferSize = static_cast<float>(m_directionalLightShadowBufferSize);
    float    modV             = sizeSplit * 2.0f / shadowBufferSize;
    float    onAxisX          = xAxis.Dot(centerPosition);
    float    onAxisY          = yAxis.Dot(centerPosition);
    float    modX             = fmodf(onAxisX, modV);
    float    modY             = fmodf(onAxisY, modV);


    calc_projection_matrix_inv(m_device, splitPoints[i], splitPoints[i + 1], -1.0f, 1.0f, view, projInv);


    CameraClipper clipper(viewInv, projInv, false, false);


    float near = FLT_MAX;
    float far  = -FLT_MAX;
    m_collector.Clear();
    m_scene->ScanMeshes(&clipper, iGfxScene::eSM_Dynamic | iGfxScene::eSM_Static, m_collector);
    std::vector<GfxMesh *> &meshes = m_collector.GetMeshes(eRenderQueue::Default);

    for (const auto &mesh: meshes)
    {
      if (mesh->IsCastShadow())
      {
        const Vector3f *bboxPoints = mesh->GetBoundingBox().GetPoints();
        for (unsigned  j           = 0; j < 8; j++)
        {
          Vector3f point = bboxPoints[j];
          float    v     = Matrix4f::TransformZ(view, point);

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
//    calc_projection_matrix(m_device, splitPoints[i], splitPoints[i + 1], near, far, view, proj);

    shadowMapView[i]       = view;
    shadowMapProjection[i] = proj;

    m_shadowMatrices[i] = proj * view;

    m_device->ResetTextures();
    m_device->SetRenderTarget(GetShadowBuffer(i));
    m_device->SetRenderBuffer(0);
    m_device->SetDepthWrite(true);
    m_device->SetDepthTest(true);
    m_device->SetBlending(false);
    m_device->SetColorWrite(false, false, false, false);
    m_device->Clear(false, Color4f(0.0f, 0.0f, 0.0f, 1.0f), true, 1.0f, false, 0);

    m_device->SetProjectionMatrix(proj);
    m_device->SetViewMatrix(view);

//    std::sort(m_meshesCache.begin(), m_meshesCache.end(), material_shader_compare_less_forward);

    uint64_t        startTime = Time::GetTime();
//    printf (" [%d @ (%.2f %.2f)]", m_meshesCache.size(), near, far);
    size_t          c         = 0;
    for (const auto &mesh: meshes)
    {
      if (mesh->IsCastShadow())
      {
        mesh->RenderUnlit(m_device, eRP_Depth);
        c++;
      }
    }
//    printf ("%d ", c);
  }
//  printf ("\n");
  // 66 430 580 401
  // 521 3599 4183 2539

  m_device->SetShadowMapProjectionMatrices(shadowMapProjection, 4);
  m_device->SetShadowMapViewMatrices(shadowMapView, 4);

}


void GL4PSSMRenderer::RenderShadowMap(const GL4DirectionalLight *directionalLight,
                                      const ce::Camera &camera,
                                      const ce::Projector &projector)
{
  m_device->ResetTextures();
  GL4RenderTarget2D *target = GetDirectionalLightShadowMapTemp();// m_directionalLightShadowMap;
//  GL4RenderTarget2D *target = m_directionalLightShadowMap;
  m_device->SetRenderTarget(target);
  m_device->SetRenderBuffer(0);
  m_device->SetDepthWrite(true);
  m_device->SetDepthTest(false);
  m_device->SetBlending(false);
  m_device->SetColorWrite(true, true, true, true);
  m_device->Clear(true, Color4f(0.0, 0.0f, 0.0f, 0.0f), true, 1.0f, true, 0);

  m_device->SetShader(m_shadowMappingShader);
  m_device->ResetTextures();
  camera.Bind(m_device);
  projector.Bind(m_device);

  if (m_attrLayersDepth)
  {
    m_attrLayersDepth->Bind(Vector4f(m_splits[0], m_splits[1], m_splits[2], m_splits[3]));
  }
  if (m_attrLayersBias)
  {
    m_attrLayersBias->Bind(directionalLight->GetShadowMapBias());
  }

  if (m_attrShadowBuffers)
  {
//    for (size_t i = 0; i < 4; i++)
    {
      eTextureUnit unit = m_device->BindTexture(GetShadowBuffer().ShadowDepth);
      m_attrShadowBuffers->SetArrayIndex(0);
      m_attrShadowBuffers->Bind(unit);
    }
  }
  if (m_attrDepthBuffer)
  {
    eTextureUnit unit = m_device->BindTexture(m_depthBuffer);
    m_attrDepthBuffer->Bind(unit);
  }

  m_device->BindMatrices();
  m_device->BindStandardValues();

  m_device->RenderFullscreen();
}

void GL4PSSMRenderer::FilterShadowMap()
{
  m_device->ResetTextures();

  m_shadowMapFilter.Render(m_device,
                           m_depthBuffer,
                           GetDirectionalLightShadowMapTemp()->GetColorTexture(0),
                           m_directionalLightShadowMap
  );

}


GL4PSSMShadowBufferObject GL4PSSMRenderer::CreateDirectionalLightShadowBuffer()
{
  GL4PSSMShadowBufferObject sbo {};

  if (m_shadowSamplingMode == ShadowSamplingMode::VSM)
  {
    iTexture2DArray::Descriptor colorDesc {};
    colorDesc.Width   = m_directionalLightShadowBufferSize;
    colorDesc.Height  = m_directionalLightShadowBufferSize;
    colorDesc.Layers  = 4;
    colorDesc.Format  = ePF_RGBA;
    colorDesc.MipMaps = false;
    sbo.ShadowColor = QueryClass<GL4Texture2DArray>(m_device->CreateTexture(colorDesc));
    sbo.ShadowColor->SetSampler(GetShadowBufferColorSampler());
  }

  iTexture2DArray::Descriptor depthDesc {};
  depthDesc.Width   = m_directionalLightShadowBufferSize;
  depthDesc.Height  = m_directionalLightShadowBufferSize;
  depthDesc.Layers  = 4;
  depthDesc.Format  = ePF_Depth;
  depthDesc.MipMaps = false;
  sbo.ShadowDepth = QueryClass<GL4Texture2DArray>(m_device->CreateTexture(depthDesc));
  sbo.ShadowDepth->SetSampler(GetShadowBufferDepthSampler());


  for (size_t i = 0, in = sbo.ShadowBuffers.size(); i < in; i++)
  {
    iRenderTarget2D::Descriptor desc {};
    desc.Width  = (uint16_t) m_directionalLightShadowBufferSize;
    desc.Height = (uint16_t) m_directionalLightShadowBufferSize;
    auto shadowRenderTarget = QueryClass<GL4RenderTarget2D>(m_device->CreateRenderTarget(desc));


    if (sbo.ShadowColor)
    {
      shadowRenderTarget->AddColorTexture(sbo.ShadowColor, i);
    }
    if (sbo.ShadowDepth)
    {
      shadowRenderTarget->SetDepthTexture(sbo.ShadowDepth, i);
    }

    if (!shadowRenderTarget->Compile())
    {
      printf("Unable to compile\n");
      shadowRenderTarget->Release();
      shadowRenderTarget = nullptr;
    }

    sbo.ShadowBuffers[i] = shadowRenderTarget;
  }
  return sbo;

}

bool GL4PSSMRenderer::IsShadowMapValid(GL4RenderTarget2D *shadowMap) const
{
  return shadowMap
         && shadowMap->GetWidth() == m_directionalLightShadowMapWidth
         && shadowMap->GetHeight() == m_directionalLightShadowMapHeight;
}

bool GL4PSSMRenderer::IsShadowBufferValid(GL4PSSMShadowBufferObject &shadowMap) const
{
  GL4RenderTarget2D *&buffer = shadowMap.ShadowBuffers[0];
  return buffer
         && buffer->GetWidth() == m_directionalLightShadowBufferSize
         && buffer->GetHeight() == m_directionalLightShadowBufferSize;
}

GL4RenderTarget2D *GL4PSSMRenderer::GetDirectionalLightShadowMapTemp()
{
  GL4RenderTarget2D *target = m_directionalLightShadowMapTemp;
  if (target)
  {
    if (m_directionalLightShadowMapWidth == target->GetWidth()
        && m_directionalLightShadowMapHeight == target->GetHeight())
    {
      return target;
    }
    target->Release();
  }

  target                          = CreateDirectionalLightShadowMap();
  m_directionalLightShadowMapTemp = target;
  return target;
}


GL4RenderTarget2D *GL4PSSMRenderer::CreateDirectionalLightShadowMap()
{
  iRenderTarget2D::Descriptor desc {};
  desc.Width  = (uint16_t) m_directionalLightShadowMapWidth;
  desc.Height = (uint16_t) m_directionalLightShadowMapHeight;
  auto target = QueryClass<GL4RenderTarget2D>(m_device->CreateRenderTarget(desc));

  iTexture2D::Descriptor colorDesc {};
  colorDesc.Width   = (uint16_t) m_directionalLightShadowMapWidth;
  colorDesc.Height  = (uint16_t) m_directionalLightShadowMapHeight;
  colorDesc.Format  = ePF_RGB;
  colorDesc.MipMaps = false;
  iTexture2D *colorTexture = m_device->CreateTexture(colorDesc);
  colorTexture->SetSampler(GetShadowMapColorSampler());
  target->AddColorTexture(colorTexture);
  target->SetDepthBuffer(ePF_Depth);

  if (!target->Compile())
  {
    target->Release();
    target = nullptr;
  }

  return target;
}


float GL4PSSMRenderer::GetSplitSize(const Vector3f *near, const Vector3f *far)
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

iSampler *GL4PSSMRenderer::GetShadowMapColorSampler()
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

iSampler *GL4PSSMRenderer::GetShadowBufferColorSampler()
{
  if (!m_shadowBufferColorSampler)
  {
    m_shadowBufferColorSampler = m_device->CreateSampler();
    m_shadowBufferColorSampler->SetFilterMode(eFM_MinMagNearest);
    m_shadowBufferColorSampler->SetAnisotropy(1);
    m_shadowBufferColorSampler->SetAddressU(eTAM_Clamp);
    m_shadowBufferColorSampler->SetAddressV(eTAM_Clamp);
    m_shadowBufferColorSampler->SetAddressW(eTAM_Clamp);
  }
  return m_shadowBufferColorSampler;
}

iSampler *GL4PSSMRenderer::GetShadowBufferDepthSampler()
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

GL4PSSMShadowBufferObject::GL4PSSMShadowBufferObject()
{
  ShadowDepth = nullptr;
  ShadowColor = nullptr;
  ShadowBuffers[0] = nullptr;
  ShadowBuffers[1] = nullptr;
  ShadowBuffers[2] = nullptr;
  ShadowBuffers[3] = nullptr;
}

GL4PSSMShadowBufferObject::GL4PSSMShadowBufferObject(const GL4PSSMShadowBufferObject &sbo)
{
  CS_SET(ShadowDepth, sbo.ShadowDepth);
  CS_SET(ShadowColor, sbo.ShadowColor);
  CS_SET(ShadowBuffers[0], sbo.ShadowBuffers[0]);
  CS_SET(ShadowBuffers[1], sbo.ShadowBuffers[1]);
  CS_SET(ShadowBuffers[2], sbo.ShadowBuffers[2]);
  CS_SET(ShadowBuffers[3], sbo.ShadowBuffers[3]);
}

GL4PSSMShadowBufferObject::GL4PSSMShadowBufferObject(GL4PSSMShadowBufferObject &&sbo)
{
  ShadowDepth = sbo.ShadowDepth;
  ShadowColor = sbo.ShadowColor;
  ShadowBuffers[0]     = sbo.ShadowBuffers[0];
  ShadowBuffers[1]     = sbo.ShadowBuffers[1];
  ShadowBuffers[2]     = sbo.ShadowBuffers[2];
  ShadowBuffers[3]     = sbo.ShadowBuffers[3];
  sbo.ShadowBuffers[0] = nullptr;
  sbo.ShadowBuffers[1] = nullptr;
  sbo.ShadowBuffers[2] = nullptr;
  sbo.ShadowBuffers[3] = nullptr;
}

GL4PSSMShadowBufferObject::~GL4PSSMShadowBufferObject()
{
  CS_RELEASE(ShadowDepth);
  CS_RELEASE(ShadowColor);
  CS_RELEASE(ShadowBuffers[0]);
  CS_RELEASE(ShadowBuffers[1]);
  CS_RELEASE(ShadowBuffers[2]);
  CS_RELEASE(ShadowBuffers[3]);
}

GL4PSSMShadowBufferObject &GL4PSSMShadowBufferObject::operator=(const GL4PSSMShadowBufferObject &sbo)
{
  CS_SET(ShadowDepth, sbo.ShadowDepth);
  CS_SET(ShadowColor, sbo.ShadowColor);
  CS_SET(ShadowBuffers[0], sbo.ShadowBuffers[0]);
  CS_SET(ShadowBuffers[1], sbo.ShadowBuffers[1]);
  CS_SET(ShadowBuffers[2], sbo.ShadowBuffers[2]);
  CS_SET(ShadowBuffers[3], sbo.ShadowBuffers[3]);
  return *this;
}

const std::array<Matrix4f, 4> &GL4PSSMRenderer::GetMatrices() const
{
  return m_shadowMatrices;
}

const std::array<float, 4> &GL4PSSMRenderer::GetSplits() const
{
  return m_splits;
}

}
