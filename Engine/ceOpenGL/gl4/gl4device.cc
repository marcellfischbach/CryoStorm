
#include <ceOpenGL/gl4/gl4device.hh>
#include <ceOpenGL/gl4/gl4comparefuncmap.hh>
#include <ceOpenGL/gl4/gl4blendfactormap.hh>
#include <ceOpenGL/gl4/gl4directionallight.hh>
#include <ceOpenGL/gl4/gl4pointlight.hh>
#include <ceOpenGL/gl4/gl4rendertarget2d.hh>
#include <ceOpenGL/gl4/gl4rendertarget2darray.hh>
#include <ceOpenGL/gl4/gl4rendertargetcube.hh>
#include <ceOpenGL/gl4/gl4rendermesh.hh>
#include <ceOpenGL/gl4/gl4sampler.hh>
#include <ceOpenGL/gl4/gl4texture2d.hh>
#include <ceOpenGL/gl4/gl4texture2darray.hh>
#include <ceOpenGL/gl4/gl4texturecube.hh>
#include <ceOpenGL/gl4/shading/gl4program.hh>
#include <ceOpenGL/glerror.hh>
#include <ceCore/objectregistry.hh>
#include <ceCore/graphics/samplers.hh>
#include <ceCore/graphics/shading/ishaderattribute.hh>
#include <ceCore/resource/assetmanager.hh>
#include <GL/glew.h>
#include <iostream>

#define CE_MAX_LIGHTS 4

namespace ce::opengl
{

GL4Device::GL4Device()
    : iDevice()
    , m_renderTarget(nullptr)
    , m_shader(nullptr)
    , m_nextTextureUnit(eTU_Unit0)
    , m_colorWrite(0x0f)
    , m_depthWrite(true)
    , m_depthTest(true)
    , m_blending(false)
    , m_srcFactorColor(eBlendFactor::One)
    , m_srcFactorAlpha(eBlendFactor::One)
    , m_dstFactorColor(eBlendFactor::Zero)
    , m_dstFactorAlpha(eBlendFactor::Zero)
    , m_modelViewMatrixDirty(false)
    , m_viewProjectionMatrixDirty(false)
    , m_modelViewProjectionMatrixDirty(false)
    , m_modelMatrixInvDirty(false)
    , m_viewMatrixInvDirty(false)
    , m_projectionMatrixInvDirty(false)
    , m_modelViewMatrixInvDirty(false)
    , m_viewProjectionMatrixInvDirty(false)
    , m_modelViewProjectionMatrixInvDirty(false)
    , m_skeletonMatrixCount(0)
    , m_clearColorR(0.0f)
    , m_clearColorG(0.0f)
    , m_clearColorB(0.0f)
    , m_clearColorA(0.0f)
    , m_clearDepth(1.0f)
    , m_clearStencil(0)
    , m_activeTexture(0)
    , m_fullscreenBlitProgram(nullptr)
    , m_fullscreenBlitMSProgram(nullptr)
    , m_fullscreenBlitRenderMesh(nullptr)
    , m_pixelRenderMesh(nullptr)
    , m_fullscreenBlitArrayProgram(nullptr)
    , m_fullscreenBlitCubeProgram(nullptr)
    , m_fullscreenBlitCubePosXRenderMesh(nullptr)
    , m_fullscreenBlitCubePosYRenderMesh(nullptr)
    , m_fullscreenBlitCubePosZRenderMesh(nullptr)
    , m_fullscreenBlitCubeNegXRenderMesh(nullptr)
    , m_fullscreenBlitCubeNegYRenderMesh(nullptr)
    , m_fullscreenBlitCubeNegZRenderMesh(nullptr)
    , m_renderLayer(-1)
{
  CE_CLASS_GEN_CONSTR;


}

GL4Device::~GL4Device()
{

}

bool GL4Device::Initialize()
{
  GLenum res = glewInit();
  if (res != GLEW_OK)
  {
    printf("Unable to initialize GLEW\n");
    return false;
  }

  GLint units, imageUnits, combinedUnits;

  glGetIntegerv(GL_MAX_TEXTURE_UNITS, &units);
  glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &imageUnits);
  glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &combinedUnits);
  printf("OpenGL capabilities:\n");
  printf("  Vendor  : %s\n", (const char *) glGetString(GL_VENDOR));
  printf("  Renderer: %s\n", (const char *) glGetString(GL_RENDERER));
  printf("  Version : %s\n", (const char *) glGetString(GL_VERSION));
  printf("  GLSL    : %s\n", (const char *) glGetString(GL_SHADING_LANGUAGE_VERSION));
  printf("  Max texture units: %d\n", units);
  printf("  Max images units: %d\n", imageUnits);
  printf("  Max combined textures: %d\n", combinedUnits);

  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glColorMask(true, true, true, true);
  glDepthMask(true);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glDisable(GL_BLEND);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBlendFuncSeparate(GL_ONE, GL_ZERO, GL_ONE, GL_ZERO);


  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CW);
  glCullFace(GL_BACK);

  m_modelMatrix.SetIdentity();
  m_viewMatrix.SetIdentity();
  m_projectionMatrix.SetIdentity();
  m_modelViewMatrix.SetIdentity();
  m_viewProjectionMatrix.SetIdentity();
  m_modelViewProjectionMatrix.SetIdentity();
  m_modelMatrixInv.SetIdentity();
  m_viewMatrixInv.SetIdentity();
  m_projectionMatrixInv.SetIdentity();
  m_modelViewMatrixInv.SetIdentity();
  m_viewProjectionMatrixInv.SetIdentity();
  m_modelViewProjectionMatrixInv.SetIdentity();

  for (Size i = 0; i < eTU_COUNT; i++)
  {
    m_textures[i] = nullptr;
    m_samplers[i] = nullptr;
  }

  for (Size i = 0; i < 256; i++)
  {
    m_skeletonMatrices[i].SetIdentity();
  }

  CE_GL_ERROR();

  return true;
}

void GL4Device::SetViewport(int16_t x, int16_t y, uint16_t width, uint16_t height)
{
  glViewport(x, y, width, height);
}

void GL4Device::Clear(bool clearColor,
                      const Color4f &color,
                      bool clearDepth,
                      float depth,
                      bool clearStencil,
                      uint8_t stencil
                     )
{
  GLenum flags = 0;
  if (clearColor)
  {
    flags |= GL_COLOR_BUFFER_BIT;
    if (color.r != m_clearColorR || color.g != m_clearColorG || color.b != m_clearColorB || color.a != m_clearColorA)
    {
      m_clearColorR = color.r;
      m_clearColorG = color.g;
      m_clearColorB = color.b;
      m_clearColorA = color.a;
      glClearColor(color.r, color.g, color.b, color.a);
    }
  }
  if (clearDepth)
  {
    flags |= GL_DEPTH_BUFFER_BIT;
    if (depth != m_clearDepth)
    {
      m_clearDepth = depth;
      glClearDepth(depth);
    }
  }
  if (clearStencil)
  {
    flags |= GL_STENCIL_BUFFER_BIT;
    if (stencil != m_clearStencil)
    {
      m_clearStencil = stencil;
      glClearStencil(stencil);
    }
  }
  if (flags != 0)
  {
    glClear(flags);
  }
}

void GL4Device::SetColorWrite(bool redWrite, bool greenWrite, bool blueWrite, bool alphaWrite)
{
  uint8_t colorWrite = 0x00
                       | (redWrite ? 0x08 : 0x00)
                       | (greenWrite ? 0x04 : 0x00)
                       | (blueWrite ? 0x02 : 0x00)
                       | (alphaWrite ? 0x01 : 0x00);
  if (m_colorWrite != colorWrite)
  {
    m_colorWrite = colorWrite;
    glColorMask(redWrite, greenWrite, blueWrite, alphaWrite);
  }

}

void GL4Device::SetDepthWrite(bool depthWrite)
{
  if (m_depthWrite != depthWrite)
  {
    m_depthWrite = depthWrite;
    glDepthMask(depthWrite);
  }
}

void GL4Device::SetDepthTest(bool depthTest)
{
  if (m_depthTest != depthTest)
  {
    m_depthTest = depthTest;
    if (depthTest)
    {
      glEnable(GL_DEPTH_TEST);
    }
    else
    {
      glDisable(GL_DEPTH_TEST);
    }
  }
}

void GL4Device::SetFillMode(eFillMode fillMode)
{
  if (m_fillMode != fillMode)
  {
    m_fillMode = fillMode;
    switch (fillMode)
    {
      case eFillMode::Wireframe:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
      case eFillMode::Fill:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
    }
  }
}

void GL4Device::SetDepthFunc(eCompareFunc func)
{
  if (m_depthFunc != func)
  {
    m_depthFunc = func;
    glDepthFunc(GL4CompareFunc[m_depthFunc]);
  }
}

void GL4Device::SetBlending(bool blending)
{
  CE_GL_ERROR()

  if (m_blending != blending)
  {
    m_blending = blending;
    if (m_blending)
    {
      glEnable(GL_BLEND);
    }
    else
    {
      glDisable(GL_BLEND);
    }
  }
  CE_GL_ERROR()
}

void GL4Device::SetBlendFactor(eBlendFactor srcFactor, eBlendFactor dstFactor)
{
  CE_GL_ERROR()
  SetBlendFactor(srcFactor, srcFactor, dstFactor, dstFactor);
  CE_GL_ERROR()
}

void GL4Device::SetBlendFactor(eBlendFactor srcFactorColor,
                               eBlendFactor srcFactorAlpha,
                               eBlendFactor dstFactorColor,
                               eBlendFactor dstFactorAlpha
                              )
{
  if (srcFactorColor != m_srcFactorColor
      || srcFactorAlpha != m_srcFactorAlpha
      || dstFactorColor != m_dstFactorColor
      || dstFactorAlpha != m_dstFactorAlpha)
  {
    m_srcFactorColor = srcFactorColor;
    m_srcFactorAlpha = srcFactorAlpha;
    m_dstFactorColor = dstFactorColor;
    m_dstFactorAlpha = dstFactorAlpha;
    GLenum glSrcColor = GL4BlendFactorMap[static_cast<int>(m_srcFactorColor)];
    GLenum glSrcAlpha = GL4BlendFactorMap[static_cast<int>(m_srcFactorAlpha)];
    GLenum glDstColor = GL4BlendFactorMap[static_cast<int>(m_dstFactorColor)];
    GLenum glDstAlpha = GL4BlendFactorMap[static_cast<int>(m_dstFactorAlpha)];
    glBlendFuncSeparate(glSrcColor, glDstColor, glSrcAlpha, glDstAlpha);
  }
}

void GL4Device::SetModelMatrix(const Matrix4f &modelMatrix)
{
  m_modelMatrix = modelMatrix;

  m_modelViewMatrixDirty           = true;
  m_modelViewProjectionMatrixDirty = true;

  m_modelMatrixInvDirty               = true;
  m_modelViewMatrixInvDirty           = true;
  m_modelViewProjectionMatrixInvDirty = true;
}

void GL4Device::SetViewMatrix(const Matrix4f &viewMatrix)
{
  m_viewMatrix = viewMatrix;

  m_modelViewMatrixDirty      = true;
  m_viewProjectionMatrixDirty = true;


  m_viewMatrixInvDirty           = true;
  m_modelViewMatrixInvDirty      = true;
  m_viewProjectionMatrixInvDirty = true;
}

void GL4Device::SetProjectionMatrix(const Matrix4f &projectionMatrix)
{
  m_projectionMatrix = projectionMatrix;

  m_viewProjectionMatrixDirty      = true;
  m_modelViewProjectionMatrixDirty = true;

  m_projectionMatrixInvDirty          = true;
  m_viewProjectionMatrixInvDirty      = true;
  m_modelViewProjectionMatrixInvDirty = true;
}

void GL4Device::SetModelMatrix(const Matrix4f &modelMatrix, const Matrix4f &modelMatrixInv)
{
  m_modelMatrix    = modelMatrix;
  m_modelMatrixInv = modelMatrixInv;

  m_modelViewMatrixDirty           = true;
  m_modelViewProjectionMatrixDirty = true;

  m_modelViewMatrixInvDirty           = true;
  m_modelViewProjectionMatrixInvDirty = true;
}

void GL4Device::SetViewMatrix(const Matrix4f &viewMatrix, const Matrix4f &viewMatrixInv)
{
  m_viewMatrix    = viewMatrix;
  m_viewMatrixInv = viewMatrixInv;

  m_modelViewMatrixDirty      = true;
  m_viewProjectionMatrixDirty = true;


  m_modelViewMatrixInvDirty      = true;
  m_viewProjectionMatrixInvDirty = true;
}

void GL4Device::SetProjectionMatrix(const Matrix4f &projectionMatrix, const Matrix4f &projectionMatrixInv)
{
  m_projectionMatrix    = projectionMatrix;
  m_projectionMatrixInv = projectionMatrixInv;

  m_viewProjectionMatrixDirty      = true;
  m_modelViewProjectionMatrixDirty = true;

  m_viewProjectionMatrixInvDirty      = true;
  m_modelViewProjectionMatrixInvDirty = true;
}

void GL4Device::SetShadowMapViewMatrices(const Matrix4f *matrices, Size numberOfMatrices)
{
  m_shadowMapMatrixCount = numberOfMatrices;
  memcpy(m_shadowMapViewMatrices, matrices, sizeof(Matrix4f) * numberOfMatrices);
  m_shadowMapViewProjectionMatrixDirty = true;
}

void GL4Device::SetShadowMapProjectionMatrices(const Matrix4f *matrices, Size numberOfMatrices)
{
  m_shadowMapMatrixCount = numberOfMatrices;
  memcpy(m_shadowMapProjectionMatrices, matrices, sizeof(Matrix4f) * numberOfMatrices);
  m_shadowMapViewProjectionMatrixDirty = true;
}

void GL4Device::SetSkeletonMatrices(const ce::Matrix4f *skeletonMatrices, Size numMatrices)
{
  Size cappedNumMatrices = ceMin(numMatrices, (Size) 256);
  memcpy(m_skeletonMatrices, skeletonMatrices, cappedNumMatrices * sizeof(Matrix4f));
  m_skeletonMatrixCount = cappedNumMatrices;
}

const Matrix4f &GL4Device::GetViewMatrix() const
{
  return m_viewMatrix;
}

const Matrix4f &GL4Device::GetViewMatrixInv() const
{
  return m_viewMatrixInv;
}

const Matrix4f &GL4Device::GetProjectionMatrix() const
{
  return m_projectionMatrix;
}

const Matrix4f &GL4Device::GetProjectionMatrixInv() const
{
  return m_projectionMatrixInv;
}

Matrix4f &GL4Device::GetPerspectiveProjection(float l, float r, float b, float t, float n, float f, Matrix4f &m)
{
  float z2 = 2.0f * n;
  float dx = r - l;
  float dy = t - b;
  float dz = f - n;
  float sx = r + l;
  float sy = t + b;
  float sz = n + f;


  m.m00 = z2 / dx;
  m.m10 = 0.0f;
  m.m20 = -sx / dx;
  m.m30 = 0.0f;

  m.m01 = 0.0f;
  m.m11 = z2 / dy;
  m.m21 = -sy / dy;
  m.m31 = 0.0f;

  m.m02 = 0.0f;
  m.m12 = 0.0f;
  m.m22 = sz / dz;
  m.m32 = -2.0f * n * f / dz;

  m.m03 = 0.0f;
  m.m13 = 0.0f;
  m.m23 = 1.0f;
  m.m33 = 0.0f;
  return m;
}

Matrix4f &GL4Device::GetPerspectiveProjectionInv(float l, float r, float b, float t, float n, float f, Matrix4f &m)
{
  float z2  = 2.0f * n;
  float dx  = r - l;
  float dy  = t - b;
  float dz  = f - n;
  float sx  = r + l;
  float sy  = t + b;
  float sz  = n + f;
  float nf2 = z2 * f;


  m.m00 = dx / z2;
  m.m10 = 0.0f;
  m.m20 = 0.0f;
  m.m30 = sx / z2;
  m.m01 = 0.0f;
  m.m11 = dy / z2;
  m.m21 = 0.0f;
  m.m31 = sy / z2;
  m.m02 = 0.0f;
  m.m12 = 0.0f;
  m.m22 = 0.0f;
  m.m32 = 1.0f;
  m.m03 = 0.0f;
  m.m13 = 0.0f;
  m.m23 = -dz / nf2;
  m.m33 = sz / nf2;


  return m;
}

Matrix4f &GL4Device::GetOrthographicProjection(float l, float r, float b, float t, float n, float f, Matrix4f &m)
{
  float dx = r - l;
  float dy = t - b;
  float dz = f - n;
  float sx = r + l;
  float sy = t + b;
  float sz = f + n;

  m.m00 = 2.0f / dx;
  m.m10 = 0.0f;
  m.m20 = 0.0f;
  m.m30 = -sx / dx;
  m.m01 = 0.0f;
  m.m11 = 2.0f / dy;
  m.m21 = 0.0f;
  m.m31 = -sy / dy;
  m.m02 = 0.0f;
  m.m12 = 0.0f;
  m.m22 = 2.0f / dz;
  m.m32 = -sz / dz;
  m.m03 = 0.0f;
  m.m13 = 0.0f;
  m.m23 = 0.0f;
  m.m33 = 1.0;
  return m;
}

Matrix4f &GL4Device::GetOrthographicProjectionInv(float l, float r, float b, float t, float n, float f, Matrix4f &m)
{
  float dx = r - l;
  float dy = t - b;
  float dz = f - n;
  float sx = r + l;
  float sy = t + b;
  float sz = n + f;

  m.m00 = dx / 2.0f;
  m.m10 = 0.0f;
  m.m20 = 0.0f;
  m.m30 = sx / 2.0f;
  m.m01 = 0.0f;
  m.m11 = dy / 2.0f;
  m.m21 = 0.0f;
  m.m31 = sy / 2.0f;
  m.m02 = 0.0f;
  m.m12 = 0.0f;
  m.m22 = dz / 2.0f;
  m.m32 = sz / 2.0f;
  m.m03 = 0.0f;
  m.m13 = 0.0f;
  m.m23 = 0.0f;
  m.m33 = 1.0;

  return m;
}


void GL4Device::SetRenderLayer(ce::int8_t renderLayer)
{
  m_renderLayer = renderLayer;
}

int8_t GL4Device::GetRenderLayer() const
{
  return m_renderLayer;
}

void GL4Device::SetShader(iShader *shader)
{
  if (shader == m_shader)
  {
    return;
  }
  m_shader = shader;
#if  _DEBUG
  m_numShaderStatesChanges++;
#endif
  if (m_shader)
  {
    GL4Program *program = static_cast<GL4Program *>(m_shader);
    CE_GL_ERROR();
    glUseProgram(program->GetName());
    CE_GL_ERROR();
  }
  else
  {
    CE_GL_ERROR();
    glUseProgram(0);
    CE_GL_ERROR();
  }

}

void GL4Device::SetRenderTarget(iRenderTarget *renderTarget)
{
  if (m_renderTarget == renderTarget)
  {
    return;
  }

  m_renderTarget = renderTarget;
  if (m_renderTarget)
  {
    switch (renderTarget->GetType())
    {
      case eTextureType::Texture2D:
      {
        GL4RenderTarget2D *rt2d = static_cast<GL4RenderTarget2D *>(renderTarget);
        rt2d->Bind();
        SetViewport(0, 0, rt2d->GetWidth(), rt2d->GetHeight());
        break;
      }
      case eTextureType::Texture2DArray:
      {
        GL4RenderTarget2DArray *rt2dArray = static_cast<GL4RenderTarget2DArray *>(renderTarget);
        rt2dArray->Bind();
        SetViewport(0, 0, rt2dArray->GetWidth(), rt2dArray->GetHeight());
        break;
      }
      case eTextureType::TextureCube:
      {
        GL4RenderTargetCube *rtcube = static_cast<GL4RenderTargetCube *>(renderTarget);
        rtcube->Bind();
        SetViewport(0, 0, rtcube->GetSize(), rtcube->GetSize());
        break;
      }
      default:
        break;
    }
  }
  else
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  /*
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);


  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CW);
  glCullFace(GL_BACK);

  glColorMask(true, true, true, true);
  glDepthMask(true);
  */
}

void GL4Device::SetRenderBuffer(uint32_t buffer)
{
  glDrawBuffer(GL_COLOR_ATTACHMENT0 + buffer);
}
void GL4Device::SetRenderBuffer(const std::vector<uint32_t> &buffer)
{
  GLenum   glBuffer[16];
  for (int i = 0; i < buffer.size(); ++i)
  {
    glBuffer[i] = GL_COLOR_ATTACHMENT0 + buffer[i];
  }
  glDrawBuffers(buffer.size(), glBuffer);
}

void GL4Device::ClearShadowMaps()
{
  m_shadowDataSize = 0;
//  m_shadowMapTextures.clear();
//  m_lightShadowMaps.clear();
}

bool GL4Device::MoreShadowMapsPossible() const
{
  return m_shadowDataSize < 4;
}

//void GL4Device::AddShadowMap(iTexture2D *shadowMap)
//{
//  m_shadowMapTextures.push_back(shadowMap);
//}
//
//iTexture2D *GL4Device::GetShadowMap(unsigned int idx)
//{
//  if (idx >= m_shadowMapTextures.size())
//  {
//    return nullptr;
//  }
//  return m_shadowMapTextures[idx];
//}

void GL4Device::SetPointLightShadowMap(size_t lightIdx,
                                       iPointLight *light,
                                       iTextureCube *shadowBufferDepth,
                                       iTextureCube *shadowBufferColor,
                                       float near,
                                       float far,
                                       float bias
                                      )
{
  if (m_shadowDataSize >= 4)
  {
    return;
  }
  LightShadowData &lsd = m_lightShadowData[m_shadowDataSize++];
  lsd.LightType                    = eLT_Point;
  lsd.Light                        = light;
  lsd.PointLight.ShadowBufferDepth = shadowBufferDepth;
  lsd.PointLight.ShadowBufferColor = shadowBufferColor;
  lsd.PointLight.Near              = near;
  lsd.PointLight.Far               = far;
  lsd.PointLight.Bias              = bias;
}

void GL4Device::AddDirectionalLightShadow(iDirectionalLight *light,
                                          iTexture2D *shadowMap,
                                          iTexture2DArray *shadowBuffersDepth,
                                          iTexture2DArray *shadowBuffersColor,
                                          const std::array<float, 4> &layers,
                                          const std::array<Matrix4f, 4> &matrices)
{
  if (m_shadowDataSize >= 4)
  {
    return;
  }
  LightShadowData &lsd = m_lightShadowData[m_shadowDataSize++];
  lsd.LightType                          = eLT_Directional;
  lsd.Light                              = light;
  lsd.ShadowMap                          = shadowMap;
  lsd.DirectionalLight.ShadowBufferDepth = shadowBuffersDepth;
  lsd.DirectionalLight.ShadowBufferColor = shadowBuffersColor;
  memcpy(lsd.DirectionalLight.Matrices, matrices.data(), sizeof(Matrix4f) * 4);
  memcpy(lsd.DirectionalLight.Layers, layers.data(), sizeof(float) * 4);
}

GL4Device::LightShadowData *GL4Device::FindLightShadowData(const iLight *light)
{
  for (size_t i = 0; i < 4; i++)
  {
    LightShadowData &lsd = m_lightShadowData[i];
    if (lsd.Light == light)
    {
      return &lsd;
    }
  }
  return nullptr;
}

//void GL4Device::SetLightShadowMap(iLight *light, iTexture2D *shadowMap)
//{
//  m_lightShadowMaps[light] = shadowMap;
//}

iSampler *GL4Device::CreateSampler()
{
  return new GL4Sampler();
}

iTexture2D *GL4Device::CreateTexture(const iTexture2D::Descriptor &descriptor)
{
  CE_GL_ERROR();
  SetActiveTexture(GL_TEXTURE0 + eTU_COUNT + 1);
  UnbindUnsafe(m_tempTexture);
  CE_GL_ERROR();

  GL4Texture2D *texture = new GL4Texture2D();
  texture->Initialize(
      descriptor.Width,
      descriptor.Height,
      descriptor.Format,
      descriptor.MipMaps,
      descriptor.MultiSamples
  );
  texture->SetSampler(ObjectRegistry::Get<Samplers>()->GetDefault());
  m_tempTexture = texture;
  return texture;
}

iTexture2DArray *GL4Device::CreateTexture(const iTexture2DArray::Descriptor &descriptor)
{
  CE_GL_ERROR();
  SetActiveTexture(GL_TEXTURE0 + eTU_COUNT + 1);
  UnbindUnsafe(m_tempTexture);
  CE_GL_ERROR();

  GL4Texture2DArray *texture = new GL4Texture2DArray();
  texture->Initialize(
      descriptor.Width,
      descriptor.Height,
      descriptor.Layers,
      descriptor.Format,
      descriptor.MipMaps
  );
  texture->SetSampler(ObjectRegistry::Get<Samplers>()->GetDefault());
  m_tempTexture = texture;
  return texture;
}

iTextureCube *GL4Device::CreateTexture(const iTextureCube::Descriptor &descriptor)
{
  CE_GL_ERROR();
  SetActiveTexture(GL_TEXTURE0 + eTU_COUNT + 1);
  UnbindUnsafe(m_tempTexture);
  CE_GL_ERROR();

  GL4TextureCube *texture = new GL4TextureCube();
  texture->Initialize(
      descriptor.Size,
      descriptor.Format,
      descriptor.MipMaps
  );
  texture->SetSampler(ObjectRegistry::Get<Samplers>()->GetDefault());
  m_tempTexture = texture;
  return texture;
}

iRenderTarget2D *GL4Device::CreateRenderTarget(const iRenderTarget2D::Descriptor &descriptor)
{
  GL4RenderTarget2D *target = new GL4RenderTarget2D();
  target->Initialize(descriptor.Width, descriptor.Height);
  // reset the rendertarget
  return target;
}

iRenderTarget2DArray *GL4Device::CreateRenderTarget(const iRenderTarget2DArray::Descriptor &descriptor)
{
  GL4RenderTarget2DArray *target = new GL4RenderTarget2DArray();
  target->Initialize(descriptor.Width, descriptor.Height, descriptor.Layer);
  // reset the rendertarget
  return target;
}

iRenderTargetCube *GL4Device::CreateRenderTarget(const iRenderTargetCube::Descriptor &descriptor)
{
  GL4RenderTargetCube *target = new GL4RenderTargetCube();
  target->Initialize(descriptor.Size);
  // reset the rendertarget
  return target;
}

iPointLight *GL4Device::CreatePointLight()
{
  return new GL4PointLight();
}

iDirectionalLight *GL4Device::CreateDirectionalLight()
{
  return new GL4DirectionalLight();
}

void GL4Device::ClearTextureCache()
{
  for (size_t i = 0; i < eTU_COUNT; i++)
  {
    m_textures[i] = nullptr;
  }
  ResetTextures();
}

void GL4Device::ResetTextures()
{
  m_nextTextureUnit = eTU_Unit0;
  m_markTextureUnit = eTU_Unit0;
}

void GL4Device::MarkTexture()
{
  m_markTextureUnit = m_nextTextureUnit;
}

void GL4Device::ResetTexturesToMark()
{
  m_nextTextureUnit = m_markTextureUnit;
}

eTextureUnit GL4Device::ShiftTextureUnit()
{
  if (m_nextTextureUnit == eTU_Invalid)
  {
    return eTU_Invalid;
  }
  eTextureUnit unit = m_nextTextureUnit;
  m_nextTextureUnit = static_cast<eTextureUnit>(m_nextTextureUnit + 1);
  return unit;
}

void GL4Device::SetSampler(eTextureUnit unit, iSampler *sampler)
{
  CE_GL_ERROR()
  if (m_samplers[unit] != sampler)
  {
    m_samplers[unit] = sampler;
    if (sampler)
    {
      static_cast<GL4Sampler *>(sampler)->Bind(unit);
    }
    else
    {
      glBindSampler(unit, 0);
    }
  }
  CE_GL_ERROR()
}


void GL4Device::BindUnsafe(iTexture *texture)
{
  CE_GL_ERROR()
  if (!texture)
  {
    return;
  }
  switch (texture->GetType())
  {
    case eTextureType::Texture1D:
      break;
    case eTextureType::Texture1DArray:
      break;
    case eTextureType::Texture2D:
      static_cast<GL4Texture2D *>(texture)->Bind();
      break;
    case eTextureType::Texture2DArray:
      static_cast<GL4Texture2DArray *>(texture)->Bind();
      break;
    case eTextureType::Texture3D:
      break;
    case eTextureType::TextureCube:
      static_cast<GL4TextureCube *>(texture)->Bind();
      break;
  }
  CE_GL_ERROR()
}

void GL4Device::UnbindUnsafe(iTexture *texture)
{
  CE_GL_ERROR()
  if (!texture)
  {
    return;
  }
  switch (texture->GetType())
  {
    case eTextureType::Texture1D:
      break;
    case eTextureType::Texture1DArray:
      break;
    case eTextureType::Texture2D:
      static_cast<GL4Texture2D *>(texture)->Unbind();
      break;
    case eTextureType::Texture2DArray:
      static_cast<GL4Texture2DArray *>(texture)->Unbind();
      break;
    case eTextureType::Texture3D:
      break;
    case eTextureType::TextureCube:
      static_cast<GL4TextureCube *>(texture)->Unbind();
      break;
  }
  CE_GL_ERROR()

}

eTextureUnit GL4Device::BindTexture(iTexture *texture)
{
#ifndef CE_DISABLE_RENDERING
  CE_GL_ERROR()

  if (!texture || m_nextTextureUnit == eTU_Invalid)
  {
    return eTU_Invalid;
  }


  static unsigned prebound = 0;
  static unsigned bound    = 0;
  bound++;
  eTextureUnit unit = ShiftTextureUnit();
  if (m_textures[unit] != texture)
  {
    iTexture *oldTexture = m_textures[unit];
    m_textures[unit] = texture;
//    CE_ADDREF(texture);

    CE_GL_ERROR()
    SetActiveTexture(GL_TEXTURE0 + unit);
    UnbindUnsafe(oldTexture);
    BindUnsafe(texture);
    CE_GL_ERROR()


    SetSampler(unit, texture->GetSampler());
    CE_GL_ERROR()
//    CE_RELEASE(oldTexture);
  }
  else
  {
    prebound++;
  }


  return unit;
#else
  return eTU_Invalid;
#endif
}

void GL4Device::SetActiveTexture(ce::uint32_t activeTexture)
{
  if (m_activeTexture != activeTexture)
  {
    glActiveTexture(activeTexture);
    m_activeTexture = activeTexture;
  }
}

bool GL4Device::BindMaterial(iMaterial *material, eRenderPass pass)
{
#ifndef CE_DISABLE_RENDERING
  CE_GL_ERROR()
  if (m_material == material && m_materialPass == pass)
  {
    ResetTexturesToMark();
    return true;
  }
  m_material     = material;
  m_materialPass = pass;
  bool res = material && material->Bind(this, pass);
  CE_GL_ERROR()
  return res;
#else
  return true;
#endif

}

void GL4Device::Render(iRenderMesh *mesh, eRenderPass pass)
{
#ifndef CE_DISABLE_RENDERING
  if (mesh)
  {
    CE_GL_ERROR();
    BindMatrices();
    BindStandardValues();
    CE_GL_ERROR();
    mesh->Render(this, pass);
    CE_GL_ERROR();
  }
#endif
}

void GL4Device::RenderPixel()
{
#ifndef CE_DISABLE_RENDERING
  iRenderMesh *mesh = PixelRenderMesh();
  mesh->Render(this, eRP_Forward);
#endif
}

void GL4Device::RenderFullscreen()
{
#ifndef CE_DISABLE_RENDERING
  iRenderMesh *mesh = FullscreenBlitRenderMesh();
  mesh->Render(this, eRP_Forward);
#endif
}


void GL4Device::RenderFullscreen(iTexture2D *texture)
{
  if (!texture)
  { return; }
#ifndef CE_DISABLE_RENDERING
  SetFillMode(eFillMode::Fill);
  bool     multiSampling = texture->IsMultiSampling();
  uint16_t samples       = texture->GetSamples();

  GL4Program *prog = multiSampling ? FullscreenBlitMSProgram() : FullscreenBlitProgram();
  SetShader(prog);
  ResetTextures();
  eTextureUnit     unit    = BindTexture(texture);
  iShaderAttribute *attrib = prog->GetShaderAttribute("Diffuse");
  if (attrib)
  {
    attrib->Bind(unit);
  }

  attrib = prog->GetShaderAttribute("Samples");
  if (attrib)
  {
    attrib->Bind(samples);
  }

  return RenderFullscreen();
#endif
}

void GL4Device::RenderFullscreen(iTexture2DArray *texture, int layer)
{
#ifndef CE_DISABLE_RENDERING
  SetFillMode(eFillMode::Fill);
  GL4Program *prog = FullscreenBlitArrayProgram();
  SetShader(prog);
  ResetTextures();
  eTextureUnit     unit    = BindTexture(texture);
  iShaderAttribute *attrib = prog->GetShaderAttribute("Diffuse");
  if (attrib)
  {
    attrib->Bind(unit);
  }
  attrib = prog->GetShaderAttribute("ArrayIndex");
  if (attrib)
  {
    attrib->Bind((float) layer);
  }
  RenderFullscreen();
#endif
}

void GL4Device::RenderFullscreen(iTextureCube
                                 *texture,
                                 eCubeFace face,
                                 const Vector2f &scale,
                                 const Vector2f &translation
                                )
{
#ifndef CE_DISABLE_RENDERING
  SetFillMode(eFillMode::Fill);
  SetDepthTest(false);
  SetDepthWrite(false);
  SetColorWrite(true, true, true, true);
  SetBlending(false);
  iRenderMesh *mesh = FullscreenBlitCubeRenderMesh((int) face);
  GL4Program  *prog = FullscreenBlitCubeProgram();
  SetShader(prog);
  ResetTextures();
  if (m_fullscreenBlitCubeDiffuse)
  {
    eTextureUnit unit = BindTexture(texture);
    m_fullscreenBlitCubeDiffuse->Bind(unit);
  }
  if (m_fullscreenBlitCubeScale)
  {
    m_fullscreenBlitCubeScale->Bind(scale);
  }
  if (m_fullscreenBlitCubeTranslation)
  {
    m_fullscreenBlitCubeTranslation->Bind(translation);
  }

  mesh->Render(this, eRP_Forward);
#endif
}

void GL4Device::BindForwardLight(const iLight *light, Size idx)
{
#ifndef CE_DISABLE_RENDERING

  if (!m_shader || idx >= CE_MAX_LIGHTS)
  {
    return;
  }

  CE_GL_ERROR();

  iShaderAttribute *lightColor      = m_shader->GetShaderAttribute(eSA_LightColor);
  iShaderAttribute *lightVector     = m_shader->GetShaderAttribute(eSA_LightVector);
  iShaderAttribute *lightRange      = m_shader->GetShaderAttribute(eSA_LightRange);
  iShaderAttribute *lightCastShadow = m_shader->GetShaderAttribute(eSA_LightCastShadow);
  iShaderAttribute *lightShadowMap  = m_shader->GetShaderAttribute(eSA_LightShadowMap);


  iShaderAttribute *dlsSplitLayers = m_shader->GetShaderAttribute(eSA_DirectionalLightShadowMapSplitLayers);
  iShaderAttribute *dlsViewProj    = m_shader->GetShaderAttribute(eSA_DirectionalLightShadowMapViewProjectionMatrix);
  iShaderAttribute *dlsDepth       = m_shader->GetShaderAttribute(eSA_DirectionalLightShadowMapDepth);
  iShaderAttribute *dlsColor       = m_shader->GetShaderAttribute(eSA_DirectionalLightShadowMapColor);
  iShaderAttribute *dlsLayersBias  = m_shader->GetShaderAttribute(eSA_DirectionalLightShadowMapLayersBias);



  if (lightColor)
  {
    lightColor->SetArrayIndex(idx);
  }
  if (lightVector)
  {
    lightVector->SetArrayIndex(idx);
  }
  if (lightRange)
  {
    lightRange->SetArrayIndex(idx);
  }
  if (lightCastShadow)
  {
    lightCastShadow->SetArrayIndex(idx);
  }
  if (lightShadowMap)
  {
    lightShadowMap->SetArrayIndex(idx);
  }




  if (light)
  {
    LightShadowData *lsd          = light->IsCastShadow() ? FindLightShadowData(light) : nullptr;
    bool            haveShadowMap = lsd != nullptr;


    if (lightColor)
    {
      lightColor->Bind(light->GetColor());
    }


    int lightCastShadowValue = 0;
    if (haveShadowMap)
    {
      lightCastShadowValue         = 1;
      iTexture2D *shadowMapTexture = lsd->ShadowMap;
      if (lightShadowMap)
      {
        eTextureUnit shadowMapUnit = BindTexture(shadowMapTexture);
        if (shadowMapUnit != eTU_Invalid)
        {
          lightShadowMap->Bind(shadowMapUnit);
        }
      }
    }
    if (lightCastShadow)
    {
      lightCastShadow->Bind(lightCastShadowValue);
    }

    switch (light->GetType())
    {
      case eLT_Point:
      {
        auto pointLight = static_cast<const iPointLight *>(light);
        if (lightVector)
        {
          lightVector->Bind(Vector4f(pointLight->GetPosition(), 1.0f));
        }
        if (lightRange)
        {
          lightRange->Bind(pointLight->GetRange());
        }
      }
        break;
      case eLT_Directional:
      {
        auto directionalLight = static_cast<const iDirectionalLight *>(light);
        if (lightVector)
        {
          lightVector->Bind(Vector4f(-directionalLight->GetDirection(), 0.0f));
        }
        if (haveShadowMap)
        {
          if (dlsSplitLayers)
          {
            dlsSplitLayers->SetArrayIndex(idx);
            dlsSplitLayers->Bind(lsd->DirectionalLight.Layers[0],
                                 lsd->DirectionalLight.Layers[1],
                                 lsd->DirectionalLight.Layers[2],
                                 lsd->DirectionalLight.Layers[3]);
          }
          if (dlsViewProj)
          {
            dlsViewProj->SetArrayIndex(idx * 4);
            dlsViewProj->Bind(reinterpret_cast<Matrix4f *>(lsd->DirectionalLight.Matrices), 4);
          }
          if (dlsDepth)
          {
            eTextureUnit unit = BindTexture(lsd->DirectionalLight.ShadowBufferDepth);
            if (unit != eTU_Invalid)
            {
              dlsDepth->SetArrayIndex(idx);
              dlsDepth->Bind(unit);
            }
          }
          if (dlsColor)
          {
            eTextureUnit unit = BindTexture(lsd->DirectionalLight.ShadowBufferColor);
            if (unit != eTU_Invalid)
            {
              dlsColor->SetArrayIndex(idx);
              dlsColor->Bind(unit);
            }
          }
          if (dlsLayersBias)
          {
            dlsLayersBias->SetArrayIndex(idx);
            dlsLayersBias->Bind(directionalLight->GetShadowMapBias());
          }

        }
        break;
      }
    }
  }
  else
  {
    if (lightColor)
    {
      lightColor->Bind(Color4f(0.0f, 0.0f, 0.0f));
    }
    if (lightVector)
    {
      lightVector->Bind(Vector4f(0.0f, 0.0f, 0.0f, 0.0f));
    }
    if (lightCastShadow)
    {
      lightCastShadow->Bind(0);
    }
  }
  CE_GL_ERROR();
#endif
}

void GL4Device::FinishForwardLights(Size numLights)
{
#ifndef CE_DISABLE_RENDERING

  CE_GL_ERROR();
  if (m_shader)
  {
    iShaderAttribute *count = m_shader->GetShaderAttribute(eSA_LightCount);
    if (count)
    {
      count->Bind((int) numLights);
    }
  }
  CE_GL_ERROR();
#endif
}

void GL4Device::BindMatrices()
{
#ifndef CE_DISABLE_RENDERING
  if (!m_shader)
  {
    return;
  }

  iShaderAttribute *attr = m_shader->GetShaderAttribute(eSA_ModelMatrix);
  if (attr)
  {
    attr->Bind(m_modelMatrix);
  }
  attr = m_shader->GetShaderAttribute(eSA_ViewMatrix);
  if (attr)
  {
    attr->Bind(m_viewMatrix);
  }
  attr = m_shader->GetShaderAttribute(eSA_ProjectionMatrix);
  if (attr)
  {
    attr->Bind(m_projectionMatrix);
  }
  attr = m_shader->GetShaderAttribute(eSA_ModelViewMatrix);
  if (attr)
  {
    if (m_modelViewMatrixDirty)
    {
      UpdateModelViewMatrix();
    }
    attr->Bind(m_modelViewMatrix);
  }
  attr = m_shader->GetShaderAttribute(eSA_ViewProjectionMatrix);
  if (attr)
  {
    if (m_viewProjectionMatrixDirty)
    {
      UpdateViewProjectionMatrix();
    }
    attr->Bind(m_viewProjectionMatrix);
  }
  attr = m_shader->GetShaderAttribute(eSA_ModelViewProjectionMatrix);
  if (attr)
  {
    if (m_modelViewProjectionMatrixDirty)
    {
      UpdateModelViewProjectionMatrix();
    }
    attr->Bind(m_modelViewProjectionMatrix);
  }

  attr = m_shader->GetShaderAttribute(eSA_ModelMatrixInv);
  if (attr)
  {
    if (m_modelMatrixInvDirty)
    {
      UpdateModelMatrixInv();
    }
    attr->Bind(m_modelMatrixInv);
  }
  attr = m_shader->GetShaderAttribute(eSA_ViewMatrixInv);
  if (attr)
  {
    if (m_viewMatrixInvDirty)
    {
      UpdateViewMatrixInv();
    }
    attr->Bind(m_viewMatrixInv);
  }
  attr = m_shader->GetShaderAttribute(eSA_ProjectionMatrixInv);
  if (attr)
  {
    if (m_projectionMatrixInvDirty)
    {
      UpdateProjectionMatrixInv();
    }
    attr->Bind(m_projectionMatrixInv);
  }
  attr = m_shader->GetShaderAttribute(eSA_ModelViewMatrixInv);
  if (attr)
  {
    if (m_modelViewMatrixInvDirty)
    {
      UpdateModelViewMatrixInv();
    }
    attr->Bind(m_modelViewMatrixInv);
  }
  attr = m_shader->GetShaderAttribute(eSA_ViewProjectionMatrixInv);
  if (attr)
  {
    if (m_viewProjectionMatrixInvDirty)
    {
      UpdateViewProjectionMatrixInv();
    }
    attr->Bind(m_viewProjectionMatrixInv);
  }
  attr = m_shader->GetShaderAttribute(eSA_ModelViewProjectionMatrixInv);
  if (attr)
  {
    if (m_modelViewProjectionMatrixInvDirty)
    {
      UpdateModelViewProjectionMatrixInv();
    }
    attr->Bind(m_modelViewProjectionMatrixInv);
  }

  attr = m_shader->GetShaderAttribute(eSA_ShadowMapViewMatrix);
  if (attr)
  {
    for (Size i = 0; i < m_shadowMapMatrixCount; i++)
    {
      attr->SetArrayIndex(i);
      attr->Bind(m_shadowMapViewMatrices[i]);
    }
  }

  attr = m_shader->GetShaderAttribute(eSA_ShadowMapProjectionMatrix);
  if (attr)
  {
    for (Size i = 0; i < m_shadowMapMatrixCount; i++)
    {
      attr->SetArrayIndex(i);
      attr->Bind(m_shadowMapProjectionMatrices[i]);
    }
  }

  attr = m_shader->GetShaderAttribute(eSA_ShadowMapViewProjectionMatrix);
  if (attr)
  {
    if (m_shadowMapViewProjectionMatrixDirty)
    {
      UpdateShadowMapViewProjectionMatrix();
    }
    for (Size i = 0; i < m_shadowMapMatrixCount; i++)
    {
      attr->SetArrayIndex(i);
      attr->Bind(m_shadowMapViewProjectionMatrices[i]);
    }
  }

  attr = m_shader->GetShaderAttribute(eSA_SkeletonMatrices);
  if (attr)
  {
    attr->Bind(m_skeletonMatrices, m_skeletonMatrixCount);
  }
#endif
}

void GL4Device::BindStandardValues()
{
  iShaderAttribute *attr = m_shader->GetShaderAttribute(eSA_Random);
  if (attr)
  {
    int rnd = rand();
    attr->Bind((float) rnd / (float) RAND_MAX);
  }

  attr = m_shader->GetShaderAttribute(eSA_RenderLayer);
  if (attr)
  {
    attr->Bind(m_renderLayer);
  }
}

void GL4Device::UpdateModelViewMatrix()
{
  m_modelViewMatrix      = m_viewMatrix * m_modelMatrix;
  m_modelViewMatrixDirty = false;
}

void GL4Device::UpdateViewProjectionMatrix()
{
  m_viewProjectionMatrix      = m_projectionMatrix * m_viewMatrix;
  m_viewProjectionMatrixDirty = false;
}

void GL4Device::UpdateModelViewProjectionMatrix()
{
  if (m_modelViewMatrixDirty)
  {
    UpdateModelViewMatrix();
  }
  m_modelViewProjectionMatrix      = m_projectionMatrix * m_modelViewMatrix;
  m_modelViewProjectionMatrixDirty = false;
}

void GL4Device::UpdateModelMatrixInv()
{
  m_modelMatrixInv      = m_modelMatrix.Inverted();
  m_modelMatrixInvDirty = false;
}

void GL4Device::UpdateViewMatrixInv()
{
  m_viewMatrixInv      = m_viewMatrix.Inverted();
  m_viewMatrixInvDirty = false;
}

void GL4Device::UpdateProjectionMatrixInv()
{
  m_projectionMatrixInv      = m_projectionMatrix.Inverted();
  m_projectionMatrixInvDirty = false;
}

void GL4Device::UpdateModelViewMatrixInv()
{
  if (m_modelViewMatrixDirty)
  {
    UpdateModelViewMatrix();
  }
  m_modelViewMatrixInv      = m_modelViewMatrix.Inverted();
  m_modelViewMatrixInvDirty = false;
}

void GL4Device::UpdateViewProjectionMatrixInv()
{
  if (m_viewProjectionMatrixDirty)
  {
    UpdateViewProjectionMatrix();
  }
  m_viewProjectionMatrixInv      = m_viewProjectionMatrix.Inverted();
  m_viewProjectionMatrixInvDirty = false;
}

void GL4Device::UpdateModelViewProjectionMatrixInv()
{
  if (m_modelViewProjectionMatrixDirty)
  {
    UpdateModelViewProjectionMatrix();
  }
  m_modelViewProjectionMatrixInv      = m_modelViewProjectionMatrix.Inverted();
  m_modelViewProjectionMatrixInvDirty = false;
}

void GL4Device::UpdateShadowMapViewProjectionMatrix()
{
  for (Size i = 0; i < m_shadowMapMatrixCount; i++)
  {
    m_shadowMapViewProjectionMatrices[i] = m_shadowMapProjectionMatrices[i] * m_shadowMapViewMatrices[i];

  }
  m_shadowMapViewProjectionMatrixDirty = false;
}

GL4Program *GL4Device::FullscreenBlitProgram()
{
  if (!m_fullscreenBlitProgram)
  {
    m_fullscreenBlitProgram = AssetManager::Get()->Load<GL4Program>("file:///engine/opengl/gl4/fullscreen_blit.shader");
  }
  return m_fullscreenBlitProgram;
}

GL4Program *GL4Device::FullscreenBlitMSProgram()
{
  if (!m_fullscreenBlitMSProgram)
  {
    m_fullscreenBlitMSProgram =
        AssetManager::Get()->Load<GL4Program>("file:///engine/opengl/gl4/fullscreen_blit_ms.shader");
  }
  return m_fullscreenBlitMSProgram;
}

GL4Program *GL4Device::FullscreenBlitArrayProgram()
{
  if (!m_fullscreenBlitArrayProgram)
  {
    m_fullscreenBlitArrayProgram = AssetManager::Get()->Load<GL4Program>(
        "file:///engine/opengl/gl4/fullscreen_blit_array.shader"
    );
  }
  return m_fullscreenBlitArrayProgram;
}

iRenderMesh *GL4Device::FullscreenBlitRenderMesh()
{
  if (!m_fullscreenBlitRenderMesh)
  {
    GL4RenderMeshGenerator gen;

    std::vector<Vector4f> vertices4;
    vertices4.push_back(Vector4f(-1.0f, -1.0f, 0.0f, 1.0f));
    vertices4.push_back(Vector4f(-1.0f, 1.0f, 0.0f, 1.0f));
    vertices4.push_back(Vector4f(1.0f, -1.0f, 0.0f, 1.0f));
    vertices4.push_back(Vector4f(1.0f, 1.0f, 0.0f, 1.0f));
    std::vector<Vector2f> uv;
    uv.push_back(Vector2f(0.0f, 0.0f));
    uv.push_back(Vector2f(0.0f, 1.0f));
    uv.push_back(Vector2f(1.0f, 0.0f));
    uv.push_back(Vector2f(1.0f, 1.0f));
    std::vector<uint32_t> indices;
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(3);
    indices.push_back(0);
    indices.push_back(3);
    indices.push_back(2);


    gen.SetVertices(vertices4);
    gen.SetUV0(uv);
    gen.SetIndices(indices);
    m_fullscreenBlitRenderMesh = gen.Generate();
  }
  return m_fullscreenBlitRenderMesh;
}


iRenderMesh *GL4Device::PixelRenderMesh()
{
  if (!m_fullscreenBlitRenderMesh)
  {
    GL4RenderMeshGenerator gen;

    std::vector<Vector4f> vertices4;
    vertices4.push_back(Vector4f(0.0f, 0.0f, 0.0f, 1.0f));
    std::vector<uint32_t> indices;
    indices.push_back(0);


    gen.SetVertices(vertices4);
    gen.SetIndices(indices);
    gen.SetPrimitiveType(ePT_Points);
    m_fullscreenBlitRenderMesh = gen.Generate();
  }
  return m_fullscreenBlitRenderMesh;
}


GL4Program *GL4Device::FullscreenBlitCubeProgram()
{
  if (!m_fullscreenBlitCubeProgram)
  {
    m_fullscreenBlitCubeProgram = AssetManager::Get()->Load<GL4Program>(
        "file:///engine/opengl/gl4/fullscreen_blit_cube.shader"
    );
    if (m_fullscreenBlitCubeProgram)
    {
      m_fullscreenBlitCubeDiffuse     = m_fullscreenBlitCubeProgram->GetShaderAttribute("Diffuse");
      m_fullscreenBlitCubeScale       = m_fullscreenBlitCubeProgram->GetShaderAttribute("Scale");
      m_fullscreenBlitCubeTranslation = m_fullscreenBlitCubeProgram->GetShaderAttribute("Translation");
    }

  }
  return m_fullscreenBlitCubeProgram;
}

iRenderMesh *GL4Device::FullscreenBlitCubeRenderMesh(int layer)
{
  switch (layer)
  {
    case 0:
      if (m_fullscreenBlitCubePosXRenderMesh)
      { return m_fullscreenBlitCubePosXRenderMesh; }
      else
      { break; }
    case 1:
      if (m_fullscreenBlitCubeNegXRenderMesh)
      { return m_fullscreenBlitCubeNegXRenderMesh; }
      else
      { break; }
    case 2:
      if (m_fullscreenBlitCubePosYRenderMesh)
      { return m_fullscreenBlitCubePosYRenderMesh; }
      else
      { break; }
    case 3:
      if (m_fullscreenBlitCubeNegYRenderMesh)
      { return m_fullscreenBlitCubeNegYRenderMesh; }
      else
      { break; }
    case 4:
      if (m_fullscreenBlitCubePosZRenderMesh)
      { return m_fullscreenBlitCubePosZRenderMesh; }
      else
      { break; }
    case 5:
      if (m_fullscreenBlitCubeNegZRenderMesh)
      { return m_fullscreenBlitCubeNegZRenderMesh; }
      else
      { break; }
    default:
      break;
  }


  GL4RenderMeshGenerator gen;
  std::vector<Vector4f>  vertices4;
  vertices4.push_back(Vector4f(-1.0f, -1.0f, 0.0f, 1.0f));
  vertices4.push_back(Vector4f(-1.0f, 1.0f, 0.0f, 1.0f));
  vertices4.push_back(Vector4f(1.0f, -1.0f, 0.0f, 1.0f));
  vertices4.push_back(Vector4f(1.0f, 1.0f, 0.0f, 1.0f));
  std::vector<uint32_t> indices;
  indices.push_back(0);
  indices.push_back(1);
  indices.push_back(3);
  indices.push_back(0);
  indices.push_back(3);
  indices.push_back(2);
  gen.SetVertices(vertices4);
  gen.SetIndices(indices);

  std::vector<Vector3f> uv;
  switch (layer)
  {
    case 0: // Positive X
      uv.push_back(Vector3f(1.0f, -1.0f, 1.0f));
      uv.push_back(Vector3f(1.0f, 1.0f, 1.0f));
      uv.push_back(Vector3f(1.0f, -1.0f, -1.0f));
      uv.push_back(Vector3f(1.0f, 1.0f, -1.0f));
      gen.SetUV0(uv);
      return (m_fullscreenBlitCubePosXRenderMesh = gen.Generate());
    case 1: // Negative X
      uv.push_back(Vector3f(-1.0f, -1.0f, -1.0f));
      uv.push_back(Vector3f(-1.0f, 1.0f, -1.0f));
      uv.push_back(Vector3f(-1.0f, -1.0f, 1.0f));
      uv.push_back(Vector3f(-1.0f, 1.0f, 1.0f));
      gen.SetUV0(uv);
      return (m_fullscreenBlitCubeNegXRenderMesh = gen.Generate());
    case 2: // Positive Y
      uv.push_back(Vector3f(-1.0f, 1.0f, -1.0f));
      uv.push_back(Vector3f(-1.0f, 1.0f, 1.0f));
      uv.push_back(Vector3f(1.0f, 1.0f, -1.0f));
      uv.push_back(Vector3f(1.0f, 1.0f, 1.0f));
      gen.SetUV0(uv);
      return (m_fullscreenBlitCubePosYRenderMesh = gen.Generate());
    case 3: // Negative Y
      uv.push_back(Vector3f(-1.0f, -1.0f, -1.0f));
      uv.push_back(Vector3f(-1.0f, -1.0f, 1.0f));
      uv.push_back(Vector3f(1.0f, -1.0f, -1.0f));
      uv.push_back(Vector3f(1.0f, -1.0f, 1.0f));
      gen.SetUV0(uv);
      return (m_fullscreenBlitCubeNegYRenderMesh = gen.Generate());
    case 4: // Positive Z
      uv.push_back(Vector3f(-1.0f, -1.0f, 1.0f));
      uv.push_back(Vector3f(-1.0f, 1.0f, 1.0f));
      uv.push_back(Vector3f(1.0f, -1.0f, 1.0f));
      uv.push_back(Vector3f(1.0f, 1.0f, 1.0f));
      gen.SetUV0(uv);
      return (m_fullscreenBlitCubePosZRenderMesh = gen.Generate());
    case 5: // Negative Z
      uv.push_back(Vector3f(1.0f, -1.0f, -1.0f));
      uv.push_back(Vector3f(1.0f, 1.0f, -1.0f));
      uv.push_back(Vector3f(-1.0f, -1.0f, -1.0f));
      uv.push_back(Vector3f(-1.0f, 1.0f, -1.0f));
      gen.SetUV0(uv);
      return (m_fullscreenBlitCubeNegZRenderMesh = gen.Generate());

  }
  return nullptr;
}


#if _DEBUG
void GL4Device::IncDrawCalls()
{
  ++m_numDrawCalls;
}

void GL4Device::IncTriangles(Size num)
{
  m_numTriangles += num;
}

void GL4Device::ResetDebug()
{
  m_numDrawCalls           = 0;
  m_numTriangles           = 0;
  m_numShaderStatesChanges = 0;
}

Size GL4Device::GetNumberOfDrawCalls() const
{
  return m_numDrawCalls;
}

Size GL4Device::GetNumberOfTriangles() const
{
  return m_numTriangles;
}

Size GL4Device::GetNumberOfShaderStateChanges() const
{
  return m_numShaderStatesChanges;
}

#endif

}