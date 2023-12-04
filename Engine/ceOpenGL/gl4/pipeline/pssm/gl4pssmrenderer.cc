
#include <ceOpenGL/gl4/pipeline/pssm/gl4pssmrenderer.hh>
#include <ceOpenGL/gl4/pipeline/forward/gl4forwardmeshsorter.hh>
#include <ceOpenGL/gl4/gl4device.hh>
#include <ceOpenGL/gl4/gl4directionallight.hh>
#include <ceOpenGL/gl4/gl4rendertarget2d.hh>
#include <ceOpenGL/gl4/gl4rendertarget2darray.hh>
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


namespace ce::opengl
{


void GL4PSSMRenderer::Initialize(ce::Settings &settings)
{
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
  float    radius         = settings.GetFloat("directional_light.shadow_map.filter.radius", 10.0f);
  float    samples        = settings.GetFloat("directional_light.shadow_map.filter.samples", 25.0f);
  float    sampleDistance = settings.GetFloat("directional_light.shadow_map.filter.sampleDistance", 0.1f);

  m_shadowMapFilter.Initialize(Vector2f(distance.x, distance.y - distance.x), radius, samples, sampleDistance);


  m_shadowMappingShader = AssetManager::Get()->Get<iShader>(
      ResourceLocator("file://${engine}/opengl/gl4/forward/directional_light_shadow_map.shader"));
  if (m_shadowMappingShader)
  {
    m_attrLayersDepth  = m_shadowMappingShader->GetShaderAttribute("LayersDepth");
    m_attrLayersBias   = m_shadowMappingShader->GetShaderAttribute("LayersBias");
    m_attrShadowBuffer = m_shadowMappingShader->GetShaderAttribute("ShadowBuffer");
    m_attrDepthBuffer  = m_shadowMappingShader->GetShaderAttribute("DepthBuffer");
  }

}


void GL4PSSMRenderer::SetDevice(GL4Device *device)
{
  CE_SET(m_device, device);
}

void GL4PSSMRenderer::SetDepthBuffer(iTexture2D *depthBuffer)
{
  CE_SET(m_depthBuffer, depthBuffer);
  m_directionalLightShadowMapWidth  = m_depthBuffer->GetWidth();
  m_directionalLightShadowMapHeight = m_depthBuffer->GetHeight();
}

void GL4PSSMRenderer::SetScene(iGfxScene *scene)
{
  CE_SET(m_scene, scene);
}

void GL4PSSMRenderer::SetShadowMap(GL4RenderTarget2D *shadowMap)
{
  CE_SET(m_directionalLightShadowMap, shadowMap);
}

GL4RenderTarget2D *GL4PSSMRenderer::GetShadowMap()
{
  return m_directionalLightShadowMap;
}


void GL4PSSMRenderer::RenderShadow(const GL4DirectionalLight *directionalLight,
                                   const ce::Camera &camera,
                                   const ce::Projector &projector)
{

  RenderShadowBuffer(directionalLight, camera, projector);
  RenderShadowMap(directionalLight, camera, projector);
//  FilterShadowMap();
}

void GL4PSSMRenderer::RenderShadowBuffer(const GL4DirectionalLight *directionalLight,
                                         const ce::Camera &camera,
                                         const ce::Projector &projector)
{


  Vector3f nearPoints[4];
  Vector3f split0Points[4];
  Vector3f split1Points[4];
  Vector3f split2Points[4];
  Vector3f split3Points[4];

  projector.GetPoints(m_shadowNear, nearPoints);
  projector.GetPoints(m_splits[0], split0Points);
  projector.GetPoints(m_splits[1], split1Points);
  projector.GetPoints(m_splits[2], split2Points);
  projector.GetPoints(m_splits[3], split3Points);

  float sizeSplit0   = GetSplitSize(nearPoints, split0Points) / 2.0f;
  float sizeSplit1   = GetSplitSize(split0Points, split1Points) / 2.0f;
  float sizeSplit2   = GetSplitSize(split1Points, split2Points) / 2.0f;
  float sizeSplit3   = GetSplitSize(split2Points, split3Points) / 2.0f;
  float sizeSplitTot = GetSplitSize(nearPoints, split3Points) / 2.0f;

  Vector3f direction = (camera.GetSpot() - camera.GetEye()).Normalize();
  Vector3f pos0      = camera.GetEye() + direction * (m_shadowNear + m_splits[0]) / 2.0f;
  Vector3f pos1      = camera.GetEye() + direction * (m_splits[0] + m_splits[1]) / 2.0f;
  Vector3f pos2      = camera.GetEye() + direction * (m_splits[1] + m_splits[2]) / 2.0f;
  Vector3f pos3      = camera.GetEye() + direction * (m_splits[2] + m_splits[3]) / 2.0f;
  Vector3f posTot    = camera.GetEye() + direction * (m_shadowNear + m_splits[3]) / 2.0f;

  Matrix4f mat;
  mat.SetLookAtInv(Vector3f(0, 0, 0), directionalLight->GetDirection(), Vector3f(0, 1, 0));
  Vector3f xAxis = mat.GetXAxis().Normalize();
  Vector3f yAxis = mat.GetYAxis().Normalize();

  auto  shadowBufferSize = static_cast<float>(m_directionalLightShadowBufferSize);
  float modV0            = sizeSplit0 * 2.0f / shadowBufferSize;
  float modV1            = sizeSplit1 * 2.0f / shadowBufferSize;
  float modV2            = sizeSplit2 * 2.0f / shadowBufferSize;
  float modV3            = sizeSplit3 * 2.0f / shadowBufferSize;
  float modVTot          = sizeSplitTot * 2.0f / shadowBufferSize;
  float onAxisX0         = xAxis.Dot(pos0);
  float onAxisY0         = yAxis.Dot(pos0);
  float onAxisX1         = xAxis.Dot(pos1);
  float onAxisY1         = yAxis.Dot(pos1);
  float onAxisX2         = xAxis.Dot(pos2);
  float onAxisY2         = yAxis.Dot(pos2);
  float onAxisX3         = xAxis.Dot(pos3);
  float onAxisY3         = yAxis.Dot(pos3);
  float onAxisXTot       = xAxis.Dot(posTot);
  float onAxisYTot       = yAxis.Dot(posTot);

  float mod0X   = fmodf(onAxisX0, modV0);
  float mod0Y   = fmodf(onAxisY0, modV0);
  float mod1X   = fmodf(onAxisX1, modV1);
  float mod1Y   = fmodf(onAxisY1, modV1);
  float mod2X   = fmodf(onAxisX2, modV2);
  float mod2Y   = fmodf(onAxisY2, modV2);
  float mod3X   = fmodf(onAxisX3, modV3);
  float mod3Y   = fmodf(onAxisY3, modV3);
  float modTotX = fmodf(onAxisXTot, modVTot);
  float modTotY = fmodf(onAxisYTot, modVTot);

  Matrix4f views[4];
  views[0].SetLookAt(pos0, pos0 + directionalLight->GetDirection(), Vector3f(0, 1, 0));
  views[1].SetLookAt(pos1, pos1 + directionalLight->GetDirection(), Vector3f(0, 1, 0));
  views[2].SetLookAt(pos2, pos2 + directionalLight->GetDirection(), Vector3f(0, 1, 0));
  views[3].SetLookAt(pos3, pos3 + directionalLight->GetDirection(), Vector3f(0, 1, 0));

  Matrix4f viewTot;
  viewTot.SetLookAtInv(posTot, posTot + directionalLight->GetDirection(), Vector3f(0, 1, 0));

  Matrix4f projectionTot;
  m_device->GetOrthographicProjectionInv(-sizeSplitTot - modTotX,
                                         sizeSplitTot - modTotX,
                                         -sizeSplitTot - modTotY,
                                         sizeSplitTot - modTotY,
                                         -1.0f,
                                         1.0f,
                                         projectionTot);


  CameraClipper clipper(viewTot, projectionTot, false, false);


  float near[] = {FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX};
  float far[]  = {-FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX};
  m_meshesCache.clear();
  m_scene->ScanMeshes(&clipper, iGfxScene::eSM_Dynamic | iGfxScene::eSM_Static,
                      [this, &views, &near, &far](GfxMesh *mesh) {
                        if (mesh->IsCastShadow())
                        {
                          const Vector3f *bboxPoints = mesh->GetBoundingBox().GetPoints();
                          for (unsigned  i           = 0; i < 8; i++)
                          {
                            Vector3f point = bboxPoints[i];
                            float    v0    = Matrix4f::TransformZ(views[0], point);
                            float    v1    = Matrix4f::TransformZ(views[1], point);
                            float    v2    = Matrix4f::TransformZ(views[2], point);
                            float    v3    = Matrix4f::TransformZ(views[3], point);

                            near[0] = ceMin(near[0], v0);
                            near[1] = ceMin(near[1], v1);
                            near[2] = ceMin(near[2], v2);
                            near[3] = ceMin(near[3], v3);
                            far[0]  = ceMax(far[0], v0);
                            far[1]  = ceMax(far[1], v1);
                            far[2]  = ceMax(far[2], v2);
                            far[3]  = ceMax(far[3], v3);
                          }
                          m_meshesCache.emplace_back(mesh);
                        }
                      }
  );

  Matrix4f projection0, projection1, projection2, projection3;
  m_device->GetOrthographicProjection(-sizeSplit0 - mod0X,
                                      sizeSplit0 - mod0X,
                                      -sizeSplit0 - mod0Y,
                                      sizeSplit0 - mod0Y,
                                      near[0],
                                      far[0],
                                      projection0);
  m_device->GetOrthographicProjection(-sizeSplit1 - mod1X,
                                      sizeSplit1 - mod1X,
                                      -sizeSplit1 - mod1Y,
                                      sizeSplit1 - mod1Y,
                                      near[1],
                                      far[1],
                                      projection1);
  m_device->GetOrthographicProjection(-sizeSplit2 - mod2X,
                                      sizeSplit2 - mod2X,
                                      -sizeSplit2 - mod2Y,
                                      sizeSplit2 - mod2Y,
                                      near[2],
                                      far[2],
                                      projection2);
  m_device->GetOrthographicProjection(-sizeSplit3 - mod3X,
                                      sizeSplit3 - mod3X,
                                      -sizeSplit3 - mod3Y,
                                      sizeSplit3 - mod3Y,
                                      near[3],
                                      far[3],
                                      projection3);
  Matrix4f projections[] = {
      projection0,
      projection1,
      projection2,
      projection3
  };


  m_device->SetRenderTarget(GetDirectionalLightShadowBuffer());
  m_device->SetRenderBuffer(0);
  m_device->SetDepthWrite(true);
  m_device->SetDepthTest(true);
  m_device->SetBlending(false);
  m_device->SetColorWrite(false, false, false, false);
  m_device->Clear(false, Color4f(0.0f, 0.0f, 0.0f, 1.0f), true, 1.0f, true, 0);

  m_device->SetShadowMapProjectionMatrices(projections, 4);
  m_device->SetShadowMapViewMatrices(views, 4);

  std::sort(m_meshesCache.begin(), m_meshesCache.end(), material_shader_compare_less_forward);

  for (auto mesh: m_meshesCache)
  {
    mesh->RenderUnlit(m_device, eRP_ShadowPSSM);
  }
  //  m_device->BindMaterial(nullptr, eRP_COUNT);
  //  m_device->SetColorWrite(true, true, true, true);

}


void GL4PSSMRenderer::RenderShadowMap(const GL4DirectionalLight *directionalLight,
                                      const ce::Camera &camera,
                                      const ce::Projector &projector)
{
  m_device->ResetTextures();
  GL4RenderTarget2D *target = m_directionalLightShadowMap;
  m_device->SetRenderTarget(target);
  m_device->SetRenderBuffer(0);
  m_device->SetDepthWrite(true);
  m_device->SetDepthTest(false);
  m_device->SetBlending(false);
  m_device->SetColorWrite(true, true, true, true);
  m_device->Clear(true, Color4f(0.0, 0.0f, 0.0f, 0.0f), true, 1.0f, true, 0);

  m_device->SetShader(m_shadowMappingShader);
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

  if (m_attrShadowBuffer)
  {
    eTextureUnit unit = m_device->BindTexture(GetDirectionalLightShadowBuffer()->GetDepthTexture());
    m_attrShadowBuffer->Bind(unit);
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


GL4RenderTarget2DArray *GL4PSSMRenderer::GetDirectionalLightShadowBuffer()
{
  if (!m_directionalLightShadowBuffer)
  {
    iRenderTarget2DArray::Descriptor desc {};
    desc.Width  = (uint16_t) m_directionalLightShadowBufferSize;
    desc.Height = (uint16_t) m_directionalLightShadowBufferSize;
    desc.Layer  = 4;
    auto shadowRenderTarget = QueryClass<GL4RenderTarget2DArray>(m_device->CreateRenderTarget(desc));


    if (m_shadowSamplingMode == ShadowSamplingMode::VSM)
    {
      iTexture2DArray::Descriptor colorDesc {};
      colorDesc.Width   = desc.Width;
      colorDesc.Height  = desc.Height;
      colorDesc.Layers  = desc.Layer;
      colorDesc.Format  = ePF_RGBA;
      colorDesc.MipMaps = false;
      iTexture2DArray *colorTexture = m_device->CreateTexture(colorDesc);
      colorTexture->SetSampler(GetShadowBufferColorSampler());
      shadowRenderTarget->AddColorTexture(colorTexture);
      colorTexture->Release();
    }

    iTexture2DArray::Descriptor depthDesc {};
    depthDesc.Width   = desc.Width;
    depthDesc.Height  = desc.Height;
    depthDesc.Layers  = desc.Layer;
    depthDesc.Format  = ePF_Depth;
    depthDesc.MipMaps = false;
    iTexture2DArray *depthTexture  = m_device->CreateTexture(depthDesc);
    depthTexture->SetSampler(GetShadowBufferDepthSampler());
    shadowRenderTarget->SetDepthTexture(depthTexture);
    depthTexture->Release();

    if (!shadowRenderTarget->Compile())
    {
      printf("Unable to compile\n");
      shadowRenderTarget->Release();
      shadowRenderTarget = nullptr;
    }
    m_directionalLightShadowBuffer = shadowRenderTarget;
  }
  return m_directionalLightShadowBuffer;
}

bool GL4PSSMRenderer::IsShadowMapValid(GL4RenderTarget2D *shadowMap) const
{
  return shadowMap
         && shadowMap->GetWidth() == m_directionalLightShadowMapWidth
         && shadowMap->GetHeight() == m_directionalLightShadowMapHeight;
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

}
