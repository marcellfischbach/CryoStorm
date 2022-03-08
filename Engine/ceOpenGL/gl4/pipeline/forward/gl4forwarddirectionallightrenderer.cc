

#include <ceOpenGL/gl4/pipeline/forward/gl4forwarddirectionallightrenderer.hh>
#include <ceOpenGL/gl4/pipeline/forward/gl4forwardmeshsorter.hh>
#include <ceOpenGL/gl4/gl4directionallight.hh>
#include <ceOpenGL/gl4/gl4rendertarget2darray.hh>

#include <ceCore/settings.hh>
#include <ceCore/graphics/camera.hh>
#include <ceCore/graphics/idevice.hh>
#include <ceCore/graphics/isampler.hh>
#include <ceCore/graphics/projector.hh>
#include <ceCore/graphics/scene/gfxmesh.hh>
#include <ceCore/graphics/scene/igfxscene.hh>
#include <ceCore/input/input.hh>
#include <ceCore/math/math.hh>
#include <ceCore/math/clipper/cameraclipper.hh>
#include <ceCore/math/clipper/multiplaneclipper.hh>
#include <ceCore/math/clipper/sphereclipper.hh>

#include <algorithm>
#include <GL/glew.h>
#include <float.h>

namespace ce::opengl
{

GL4ForwardDirectionalLightRenderer::GL4ForwardDirectionalLightRenderer()
    : m_device(nullptr)
      , m_shadowMapColorSampler(nullptr)
      , m_shadowMapDepthSampler(nullptr)
{

}

GL4ForwardDirectionalLightRenderer::~GL4ForwardDirectionalLightRenderer()
{
  CE_RELEASE(m_shadowMapColorSampler);
  CE_RELEASE(m_shadowMapDepthSampler);
}

void GL4ForwardDirectionalLightRenderer::Initialize(Settings &settings)
{
  m_shadowNear = settings.GetFloat("directional_light.shadow_map.near", 1.0f);
  m_shadowFar = settings.GetFloat("directional_light.shadow_map.far", 1.0f);
  Vector3f splits = settings.GetVector3f("directional_light.shadow_map.cascades", Vector3f(0.0f, 0.0f, 0.0f));
  if (splits.z == 0.0f)
  {
    splits.z = 1.0f - splits.x - splits.y;
  }
  m_splits[0] = m_shadowNear + (m_shadowFar - m_shadowNear) * splits.x;
  m_splits[1] = m_splits[0] + (m_shadowFar - m_shadowNear) * splits.y;
  m_splits[2] = m_splits[1] + (m_shadowFar - m_shadowNear) * splits.z;


  m_directionalLightShadowMapSize = settings.GetInt("directional_light.shadow_map.size", 1024);
  std::string filter = settings.GetText("directional_light.shadow_map.filter", "PCF");
  if (filter == std::string("Plain"))
  {
    m_shadowMapFilter = ShadowMapFilter::Plain;
  }
  else if (filter == std::string("PCF"))
  {
    m_shadowMapFilter = ShadowMapFilter::PCF;
  }
  else if (filter == std::string("VSM"))
  {
    m_shadowMapFilter = ShadowMapFilter::VSM;
  }
}

void GL4ForwardDirectionalLightRenderer::SetDevice(iDevice *device)
{
  m_device = device;
}

void GL4ForwardDirectionalLightRenderer::SetScene(iGfxScene *scene)
{
  m_scene = scene;
}

iTexture2DArray *GL4ForwardDirectionalLightRenderer::GetColorTexture()
{
  return !m_directionalLightShadowMap.empty() && m_directionalLightShadowMap[0]
         ? m_directionalLightShadowMap[0]->GetColorTexture(0)
         : nullptr;
}

iTexture2DArray *GL4ForwardDirectionalLightRenderer::GetDepthTexture()
{
  return !m_directionalLightShadowMap.empty() && m_directionalLightShadowMap[0]
         ? m_directionalLightShadowMap[0]->GetDepthTexture()
         : nullptr;
}

void GL4ForwardDirectionalLightRenderer::Clear()
{
  m_shadowDirectionalLights.clear();
}

void GL4ForwardDirectionalLightRenderer::Add(GL4DirectionalLight *directionalLight)
{
  if (directionalLight)
  {
    m_shadowDirectionalLights.push_back(directionalLight);
  }
}

Size GL4ForwardDirectionalLightRenderer::RenderShadowMaps(Size maxShadowLights, const Camera &camera, const Projector &projector)
{
  SortLights();
  Size i = 0;
  for (auto directionalLight : m_shadowDirectionalLights)
  {
    if (i >= maxShadowLights)
    {
      break;
    }

    GL4RenderTarget2DArray *shadowMap = GetDirectionalLightShadowMap(i);
    if (!shadowMap)
    {
      break;
    }
    RenderDirectionalShadowMaps(directionalLight, shadowMap, camera, projector);
    m_device->SetDirectionalLightShadowMap(
        directionalLight,
        Vector3f(m_splits[0], m_splits[1], m_splits[2]),
        shadowMap->GetColorTexture(0),
        shadowMap->GetDepthTexture(),
        m_shadowMatrices,
        directionalLight->GetShadowMapBias()
    );
    ++i;
  }
  return i;
}


void GL4ForwardDirectionalLightRenderer::SortLights()
{

  std::sort(m_shadowDirectionalLights.begin(), m_shadowDirectionalLights.end(),
            [](GL4DirectionalLight *light0, GL4DirectionalLight *light1) {
              return light0->GetIntensity() > light1->GetIntensity();
            });
}

GL4RenderTarget2DArray *GL4ForwardDirectionalLightRenderer::GetDirectionalLightShadowMap(Size idx)
{


  if (m_directionalLightShadowMap.size() <= idx)
  {
    iRenderTarget2DArray::Descriptor desc{};
    desc.Width = (uint16_t) m_directionalLightShadowMapSize;
    desc.Height = (uint16_t) m_directionalLightShadowMapSize;
    desc.Layer = 3;
    GL4RenderTarget2DArray *shadowRenderTarget = static_cast<GL4RenderTarget2DArray *>(m_device->CreateRenderTarget(desc));


    if (m_shadowMapFilter == ShadowMapFilter::VSM)
    {
      iTexture2DArray::Descriptor colorDesc{};
      colorDesc.Width = desc.Width;
      colorDesc.Height = desc.Height;
      colorDesc.Layers = desc.Layer;
      colorDesc.Format = ePF_RGBA;
      colorDesc.MipMaps = false;
      iTexture2DArray *colorTexture = m_device->CreateTexture(colorDesc);
      colorTexture->SetSampler(GetShadowMapColorSampler());
      shadowRenderTarget->AddColorTexture(colorTexture);
      colorTexture->Release();
    }

    iTexture2DArray::Descriptor depthDesc{};
    depthDesc.Width = desc.Width;
    depthDesc.Height = desc.Height;
    depthDesc.Layers = desc.Layer;
    depthDesc.Format = ePF_Depth;
    depthDesc.MipMaps = false;
    iTexture2DArray *depthTexture = m_device->CreateTexture(depthDesc);
    depthTexture->SetSampler(GetShadowMapDepthSampler());
    shadowRenderTarget->SetDepthTexture(depthTexture);
    depthTexture->Release();

    if (!shadowRenderTarget->Compile())
    {
      shadowRenderTarget->Release();
      shadowRenderTarget = nullptr;
      return nullptr;
    }
    else
    {
      m_directionalLightShadowMap.push_back(shadowRenderTarget);
    }
  }
  return m_directionalLightShadowMap[idx];
}

iSampler *GL4ForwardDirectionalLightRenderer::GetShadowMapColorSampler()
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


iSampler *GL4ForwardDirectionalLightRenderer::GetShadowMapDepthSampler()
{
  if (!m_shadowMapDepthSampler)
  {
    m_shadowMapDepthSampler = m_device->CreateSampler();
    if (m_shadowMapFilter == ShadowMapFilter::PCF)
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


void GL4ForwardDirectionalLightRenderer::RenderDirectionalShadowMaps(GL4DirectionalLight *directionalLight, GL4RenderTarget2DArray *shadowMap, const Camera &camera, const Projector &projector)
{
  m_device->SetRenderTarget(shadowMap);
  m_device->Clear(false, Color4f(0.0f, 0.0f, 0.0f, 1.0f), true, 1.0f, false, 0);


  Vector3f nearPoints[4];
  Vector3f split0Points[4];
  Vector3f split1Points[4];
  Vector3f split2Points[4];

  projector.GetPoints(m_shadowNear, nearPoints);
  projector.GetPoints(m_splits[0], split0Points);
  projector.GetPoints(m_splits[1], split1Points);
  projector.GetPoints(m_splits[2], split2Points);

  float sizeSplit0 = GetSplitSize(nearPoints, split0Points) / 2.0f;
  float sizeSplit1 = GetSplitSize(split0Points, split1Points) / 2.0f;
  float sizeSplit2 = GetSplitSize(split1Points, split2Points) / 2.0f;
  float sizeSplitTot = GetSplitSize(nearPoints, split2Points) / 2.0f;

  Vector3f direction = (camera.GetSpot() - camera.GetEye()).Normalize();
  Vector3f pos0 = camera.GetEye() + direction * (m_shadowNear + m_splits[0]) / 2.0f;
  Vector3f pos1 = camera.GetEye() + direction * (m_splits[0] + m_splits[1]) / 2.0f;
  Vector3f pos2 = camera.GetEye() + direction * (m_splits[1] + m_splits[2]) / 2.0f;
  Vector3f posTot = camera.GetEye() + direction * (m_shadowNear + m_splits[2]) / 2.0f;

  Matrix4f mat;
  mat.SetLookAtInv(Vector3f(0, 0, 0), directionalLight->GetDirection(), Vector3f(0, 1, 0));
  Vector3f xAxis = mat.GetXAxis().Normalize();
  Vector3f yAxis = mat.GetYAxis().Normalize();


  float modV0 = sizeSplit0 * 2.0f / m_directionalLightShadowMapSize;
  float modV1 = sizeSplit1 * 2.0f / m_directionalLightShadowMapSize;
  float modV2 = sizeSplit2 * 2.0f / m_directionalLightShadowMapSize;
  float modVTot = sizeSplitTot * 2.0f / m_directionalLightShadowMapSize;
  float onAxisX0 = xAxis.Dot(pos0);
  float onAxisY0 = yAxis.Dot(pos0);
  float onAxisX1 = xAxis.Dot(pos1);
  float onAxisY1 = yAxis.Dot(pos1);
  float onAxisX2 = xAxis.Dot(pos2);
  float onAxisY2 = yAxis.Dot(pos2);
  float onAxisXTot = xAxis.Dot(posTot);
  float onAxisYTot = yAxis.Dot(posTot);


  float mod0X = fmodf(onAxisX0, modV0);
  float mod0Y = fmodf(onAxisY0, modV0);
  float mod1X = fmodf(onAxisX1, modV1);
  float mod1Y = fmodf(onAxisY1, modV1);
  float mod2X = fmodf(onAxisX2, modV2);
  float mod2Y = fmodf(onAxisY2, modV2);
  float modTotX = fmodf(onAxisXTot, modVTot);
  float modTotY = fmodf(onAxisYTot, modVTot);


  Matrix4f views[3];
  views[0].SetLookAt(pos0, pos0 + directionalLight->GetDirection(), Vector3f(0, 1, 0));
  views[1].SetLookAt(pos1, pos1 + directionalLight->GetDirection(), Vector3f(0, 1, 0));
  views[2].SetLookAt(pos2, pos2 + directionalLight->GetDirection(), Vector3f(0, 1, 0));

  Matrix4f viewTot;
  viewTot.SetLookAtInv(posTot, posTot + directionalLight->GetDirection(), Vector3f(0, 1, 0));

  Matrix4f projectionTot;
  m_device->GetOrthographicProjectionInv(-sizeSplitTot - modTotX, sizeSplitTot - modTotX, -sizeSplitTot - modTotY, sizeSplitTot - mod2Y, -1.0f, 1.0f, projectionTot);


  CameraClipper clpr(viewTot, projectionTot, false, false);

  float near[] = {FLT_MAX, FLT_MAX, FLT_MAX};
  float far[] = {-FLT_MAX, -FLT_MAX, -FLT_MAX};
  m_meshesCache.clear();
  m_scene->ScanMeshes(&clpr, iGfxScene::eSM_Dynamic | iGfxScene::eSM_Static,
                      [this, &views, &near, &far](GfxMesh *mesh) {
                        if (mesh->IsCastShadow())
                        {
                          const Vector3f *bboxPoints = mesh->GetBoundingBox().GetPoints();
                          for (unsigned  i           = 0; i < 8; i++)
                          {
                            Vector3f point = bboxPoints[i];
                            float v0 = Matrix4f::TransformZ(views[0], point);
                            float v1 = Matrix4f::TransformZ (views[1], point);
                            float v2 = Matrix4f::TransformZ(views[2], point);

                            near[0] = ceMin(near[0], v0);
                            near[1] = ceMin(near[1], v1);
                            near[2] = ceMin(near[2], v2);
                            far[0]  = ceMax(far[0], v0);
                            far[1]  = ceMax(far[1], v1);
                            far[2]  = ceMax(far[2], v2);
                          }
                          m_meshesCache.emplace_back(mesh);
                        }
                      }
  );

  Matrix4f projection0, projection1, projection2;
  m_device->GetOrthographicProjection(-sizeSplit0 - mod0X, sizeSplit0 - mod0X, -sizeSplit0 - mod0Y, sizeSplit0 - mod0Y, near[0], far[0], projection0);
  m_device->GetOrthographicProjection(-sizeSplit1 - mod1X, sizeSplit1 - mod1X, -sizeSplit1 - mod1Y, sizeSplit1 - mod1Y, near[1], far[1], projection1);
  m_device->GetOrthographicProjection(-sizeSplit2 - mod2X, sizeSplit2 - mod2X, -sizeSplit2 - mod2Y, sizeSplit2 - mod2Y, near[2], far[2], projection2);
  Matrix4f projections[] = {
      projection0,
      projection1,
      projection2
  };


  m_shadowMatrices[0] = projections[0] * views[0];
  m_shadowMatrices[1] = projections[1] * views[1];
  m_shadowMatrices[2] = projections[2] * views[2];


  m_device->SetShadowMapProjectionMatrices(projections, 3);
  m_device->SetShadowMapViewMatrices(views, 3);

  m_device->SetColorWrite(false, false, false, false);
  std::sort(m_meshesCache.begin(), m_meshesCache.end(), material_shader_compare_less_forward);

  for (auto mesh : m_meshesCache)
  {
    mesh->RenderUnlit(m_device, eRP_ShadowPSSM);
  }
  m_device->BindMaterial(nullptr, eRP_COUNT);
  m_device->SetColorWrite(true, true, true, true);
}

float GL4ForwardDirectionalLightRenderer::GetSplitSize(const Vector3f *near, const Vector3f *far)
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
  Vector3f tmp;
  float dotFar03 = (far[0] - far[3]).Dot();
  float dotFar21 = (far[2] - far[1]).Dot();
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


}