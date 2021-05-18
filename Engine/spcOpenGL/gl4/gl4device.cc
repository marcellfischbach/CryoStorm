
#include <spcOpenGL/gl4/gl4device.hh>
#include <spcOpenGL/gl4/gl4directionallight.hh>
#include <spcOpenGL/gl4/gl4pointlight.hh>
#include <spcOpenGL/gl4/gl4rendertarget2d.hh>
#include <spcOpenGL/gl4/gl4rendertarget2darray.hh>
#include <spcOpenGL/gl4/gl4rendertargetcube.hh>
#include <spcOpenGL/gl4/gl4rendermesh.hh>
#include <spcOpenGL/gl4/gl4sampler.hh>
#include <spcOpenGL/gl4/gl4texture2d.hh>
#include <spcOpenGL/gl4/gl4texture2darray.hh>
#include <spcOpenGL/gl4/gl4texturecube.hh>
#include <spcOpenGL/gl4/shading/gl4program.hh>
#include <spcOpenGL/glerror.hh>
#include <spcCore/objectregistry.hh>
#include <spcCore/graphics/samplers.hh>
#include <spcCore/graphics/shading/ishaderattribute.hh>
#include <spcCore/resource/assetmanager.hh>
#include <GL/glew.h>
#include <iostream>

#define SPC_MAX_LIGHTS 4

namespace spc::opengl
{

GL4Device::GL4Device()
  : iDevice()
  , m_renderTarget(nullptr)
  , m_shader(nullptr)
  , m_modelViewMatrixDirty(false)
  , m_viewProjectionMatrixDirty(false)
  , m_modelViewProjectionMatrixDirty(false)
  , m_modelMatrixInvDirty(false)
  , m_viewMatrixInvDirty(false)
  , m_projectionMatrixInvDirty(false)
  , m_modelViewMatrixInvDirty(false)
  , m_viewProjectionMatrixInvDirty(false)
  , m_modelViewProjectionMatrixInvDirty(false)
  , m_fullscreenBlitProgram(nullptr)
  , m_fullscreenBlitRenderMesh(nullptr)
  , m_fullscreenBlitArrayProgram(nullptr)
  , m_fullscreenBlitCubeProgram(nullptr)
  , m_fullscreenBlitCubePosXRenderMesh(nullptr)
  , m_fullscreenBlitCubePosYRenderMesh(nullptr)
  , m_fullscreenBlitCubePosZRenderMesh(nullptr)
  , m_fullscreenBlitCubeNegXRenderMesh(nullptr)
  , m_fullscreenBlitCubeNegYRenderMesh(nullptr)
  , m_fullscreenBlitCubeNegZRenderMesh(nullptr)
{
  SPC_CLASS_GEN_CONSTR;
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
  printf("  Vendor  : %s\n", (const char*)glGetString(GL_VENDOR));
  printf("  Renderer: %s\n", (const char*)glGetString(GL_RENDERER));
  printf("  Version : %s\n", (const char*)glGetString(GL_VERSION));
  printf("  GLSL    : %s\n", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
  printf("  Max texture units: %d\n", units);
  printf("  Max images units: %d\n", imageUnits);
  printf("  Max combined textures: %d\n", combinedUnits);

  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glEnable(GL_DEPTH_TEST);


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


  SPC_GL_ERROR();

  return true;
}

void GL4Device::SetViewport(Int16 x, Int16 y, UInt16 width, UInt16 height)
{
  glViewport(x, y, width, height);
}

void GL4Device::Clear(bool clearColor,
  const Color4f& color,
  bool clearDepth,
  float depth,
  bool clearStencil,
  UInt8 stencil)
{
  GLenum flags = 0;
  if (clearColor)
  {
    flags |= GL_COLOR_BUFFER_BIT;
    glClearColor(color.r, color.g, color.b, color.a);
  }
  if (clearDepth)
  {
    flags |= GL_DEPTH_BUFFER_BIT;
    glClearDepth(depth);
  }
  if (clearStencil)
  {
    flags |= GL_STENCIL_BUFFER_BIT;
    glClearStencil(stencil);
  }
  glClear(flags);
}

void GL4Device::SetModelMatrix(const Matrix4f& modelMatrix)
{
  m_modelMatrix = modelMatrix;

  m_modelViewMatrixDirty = true;
  m_modelViewProjectionMatrixDirty = true;

  m_modelMatrixInvDirty = true;
  m_modelViewMatrixInvDirty = true;
  m_modelViewProjectionMatrixInvDirty = true;
}

void GL4Device::SetViewMatrix(const Matrix4f& viewMatrix)
{
  m_viewMatrix = viewMatrix;

  m_modelViewMatrixDirty = true;
  m_viewProjectionMatrixDirty = true;


  m_viewMatrixInvDirty = true;
  m_modelViewMatrixInvDirty = true;
  m_viewProjectionMatrixInvDirty = true;
}

void GL4Device::SetProjectionMatrix(const Matrix4f& projectionMatrix)
{
  m_projectionMatrix = projectionMatrix;

  m_viewProjectionMatrixDirty = true;
  m_modelViewProjectionMatrixDirty = true;

  m_projectionMatrixInvDirty = true;
  m_viewProjectionMatrixInvDirty = true;
  m_modelViewProjectionMatrixInvDirty = true;
}


void GL4Device::SetModelMatrix(const Matrix4f& modelMatrix, const Matrix4f& modelMatrixInv)
{
  m_modelMatrix = modelMatrix;
  m_modelMatrixInv = modelMatrixInv;

  m_modelViewMatrixDirty = true;
  m_modelViewProjectionMatrixDirty = true;

  m_modelViewMatrixInvDirty = true;
  m_modelViewProjectionMatrixInvDirty = true;
}

void GL4Device::SetViewMatrix(const Matrix4f& viewMatrix, const Matrix4f& viewMatrixInv)
{
  m_viewMatrix = viewMatrix;
  m_viewMatrixInv = viewMatrixInv;

  m_modelViewMatrixDirty = true;
  m_viewProjectionMatrixDirty = true;


  m_modelViewMatrixInvDirty = true;
  m_viewProjectionMatrixInvDirty = true;
}

void GL4Device::SetProjectionMatrix(const Matrix4f& projectionMatrix, const Matrix4f& projectionMatrixInv)
{
  m_projectionMatrix = projectionMatrix;
  m_projectionMatrixInv = projectionMatrixInv;

  m_viewProjectionMatrixDirty = true;
  m_modelViewProjectionMatrixDirty = true;

  m_viewProjectionMatrixInvDirty = true;
  m_modelViewProjectionMatrixInvDirty = true;
}

void GL4Device::SetShadowMapViewMatrices(const Matrix4f* matrices, Size numberOfMatrices)
{
  m_shadowMapMatrixCount = numberOfMatrices;
  memcpy(m_shadowMapViewMatrices, matrices, sizeof(Matrix4f) * numberOfMatrices);
  m_shadowMapViewProjectionMatrixDirty = true;
}

void GL4Device::SetShadowMapProjectionMatrices(const Matrix4f* matrices, Size numberOfMatrices)
{
  m_shadowMapMatrixCount = numberOfMatrices;
  memcpy(m_shadowMapProjectionMatrices, matrices, sizeof(Matrix4f) * numberOfMatrices);
  m_shadowMapViewProjectionMatrixDirty = true;
}


Matrix4f& GL4Device::GetPerspectiveProjection(float l, float r, float b, float t, float n, float f, Matrix4f& m)
{
  float z2 = 2.0f * n;
  float dx = r - l;
  float dy = t - b;
  float dz = f - n;
  float sx = r + l;
  float sy = t + b;
  float sz = n + f;


  m.m00 = z2 / dx; m.m10 = 0.0f;    m.m20 = -sx / dx; m.m30 = 0.0f;
  m.m01 = 0.0f;    m.m11 = z2 / dy; m.m21 = -sy / dy; m.m31 = 0.0f;
  m.m02 = 0.0f;    m.m12 = 0.0f;    m.m22 = sz / dz;  m.m32 = -2.0f * n * f / dz;
  m.m03 = 0.0f;    m.m13 = 0.0f;    m.m23 = 1.0f;     m.m33 = 0.0f;
  return m;
}

Matrix4f& GL4Device::GetPerspectiveProjectionInv(float l, float r, float b, float t, float n, float f, Matrix4f& m)
{
  float z2 = 2.0f * n;
  float dx = r - l;
  float dy = t - b;
  float dz = f - n;
  float sx = r + l;
  float sy = t + b;
  float sz = n + f;
  float nf2 = z2 * f;


  m.m00 = dx / z2;  m.m10 = 0.0f;     m.m20 = 0.0f;       m.m30 = sx / z2;
  m.m01 = 0.0f;     m.m11 = dy / z2;  m.m21 = 0.0f;       m.m31 = sy / z2;
  m.m02 = 0.0f;     m.m12 = 0.0f;     m.m22 = 0.0f;       m.m32 = 1.0f;
  m.m03 = 0.0f;     m.m13 = 0.0f;     m.m23 = -dz / nf2;  m.m33 = sz / nf2;


  return m;
}

Matrix4f& GL4Device::GetOrthographicProjection(float l, float r, float b, float t, float n, float f, Matrix4f& m)
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

Matrix4f& GL4Device::GetOrthographicProjectionInv(float l, float r, float b, float t, float n, float f, Matrix4f& m)
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


void GL4Device::SetShader(iShader* shader)
{
  if (shader == m_shader && false)
  {
    return;
  }
  m_shader = shader;
  if (m_shader)
  {
    GL4Program* program = static_cast<GL4Program*>(m_shader);
    glUseProgram(program->GetName());
  }
  else
  {
    glUseProgram(0);
  }

}

void GL4Device::SetRenderTarget(iRenderTarget* renderTarget)
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
    case eTT_Texture2D:
      {
        GL4RenderTarget2D* rt2d = static_cast<GL4RenderTarget2D*>(renderTarget);
        rt2d->Bind();
        SetViewport(0, 0, rt2d->GetWidth(), rt2d->GetHeight());
        break;
      }
    case eTT_Texture2DArray:
      {
        GL4RenderTarget2DArray* rt2dArray = static_cast<GL4RenderTarget2DArray*>(renderTarget);
        rt2dArray->Bind();
        SetViewport(0, 0, rt2dArray->GetWidth(), rt2dArray->GetHeight());
        break;
      }
    case eTT_TextureCube:
      {
        GL4RenderTargetCube* rtcube = static_cast<GL4RenderTargetCube*>(renderTarget);
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


void GL4Device::ClearShadowMaps()
{
  m_pointLightShadowData.clear();
  m_directionalLightShadowData.clear();
}

void GL4Device::SetPointLightShadowMap(iLight* light, iTextureCube* colorMap, iTextureCube* depthMap, float near, float far, float bias)
{
  PointLightShadowData data{};
  data.Light = light;
  data.Color = colorMap;
  data.Depth = depthMap;
  data.Mapping.Set(near, far, bias);
  m_pointLightShadowData[light] = data;
}

void GL4Device::SetDirectionalLightShadowMap(iLight* light, const Vector3f& layers, iTexture2DArray* colorMap, iTexture2DArray* depthMap, Matrix4f matrices[3], float bias)
{
  DirectionalLightShadowData data{};
  data.Light = light;
  data.Color = colorMap;
  data.Depth = depthMap;
  data.LayersBias.Set(layers.x, layers.y, layers.z, bias);;
  memcpy(data.Matrices, matrices, sizeof(Matrix4f) * 3);
  m_directionalLightShadowData[light] = data;
}



iSampler* GL4Device::CreateSampler()
{
  return new GL4Sampler();
}

iTexture2D* GL4Device::CreateTexture(const iTexture2D::Descriptor& descriptor)
{
  GL4Texture2D* texture = new GL4Texture2D();
  texture->Initialize(
    descriptor.Width,
    descriptor.Height,
    descriptor.Format,
    descriptor.MipMaps);
  texture->SetSampler(ObjectRegistry::Get<Samplers>()->GetDefault());
  return texture;
}


iTexture2DArray* GL4Device::CreateTexture(const iTexture2DArray::Descriptor& descriptor)
{
  GL4Texture2DArray* texture = new GL4Texture2DArray();
  texture->Initialize(
    descriptor.Width,
    descriptor.Height,
    descriptor.Layers,
    descriptor.Format,
    descriptor.MipMaps);
  texture->SetSampler(ObjectRegistry::Get<Samplers>()->GetDefault());
  return texture;
}



iTextureCube* GL4Device::CreateTexture(const iTextureCube::Descriptor& descriptor)
{
  GL4TextureCube* texture = new GL4TextureCube();
  texture->Initialize(
    descriptor.Size,
    descriptor.Format,
    descriptor.MipMaps);
  texture->SetSampler(ObjectRegistry::Get<Samplers>()->GetDefault());
  return texture;
}

iRenderTarget2D* GL4Device::CreateRenderTarget(const iRenderTarget2D::Descriptor& descriptor)
{
  GL4RenderTarget2D* target = new GL4RenderTarget2D();
  target->Initialize(descriptor.Width, descriptor.Height);
  return target;
}


iRenderTarget2DArray* GL4Device::CreateRenderTarget(const iRenderTarget2DArray::Descriptor& descriptor)
{
  GL4RenderTarget2DArray* target = new GL4RenderTarget2DArray();
  target->Initialize(descriptor.Width, descriptor.Height, descriptor.Layer);
  return target;
}

iRenderTargetCube* GL4Device::CreateRenderTarget(const iRenderTargetCube::Descriptor& descriptor)
{
  GL4RenderTargetCube* target = new GL4RenderTargetCube();
  target->Initialize(descriptor.Size);
  return target;
}


iPointLight* GL4Device::CreatePointLight()
{
  return new GL4PointLight();
}

iDirectionalLight* GL4Device::CreateDirectionalLight()
{
  return new GL4DirectionalLight();
}

void GL4Device::ResetTextures()
{
  m_nextTextureUnit = eTU_Unit0;
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

void GL4Device::SetSampler(eTextureUnit unit, iSampler* sampler)
{
  if (sampler)
  {
    static_cast<GL4Sampler*>(sampler)->Bind(unit);
  }
}


eTextureUnit GL4Device::BindTexture(iTexture* texture)
{
  if (!texture || m_nextTextureUnit == eTU_Invalid)
  {
    return eTU_Invalid;
  }


  // TODO keep track of the texture units... store them in arrays to not bind the same texture to a unity twice
  eTextureUnit unit = ShiftTextureUnit();
  glActiveTexture(GL_TEXTURE0 + unit);
  SetSampler(unit, texture->GetSampler());
  switch (texture->GetType())
  {
  case eTT_Texture2D:
    static_cast<GL4Texture2D*>(texture)->Bind();
    break;
  case eTT_Texture2DArray:
    static_cast<GL4Texture2DArray*>(texture)->Bind();
    break;

  case eTT_TextureCube:
    static_cast<GL4TextureCube*>(texture)->Bind();
    break;

  }


  return unit;
}

bool GL4Device::BindMaterial(iMaterial* material, eRenderPass pass)
{
  return material && material->Bind(this, pass);
}

void GL4Device::Render(iRenderMesh* mesh, eRenderPass pass)
{
  if (mesh)
  {
    SPC_GL_ERROR();
    BindMatrices();
    SPC_GL_ERROR();
    mesh->Render(this, pass);
    SPC_GL_ERROR();
  }
}

void GL4Device::RenderFullscreen(iTexture2D* texture)
{
  iRenderMesh* mesh = FullscreenBlitRenderMesh();
  GL4Program* prog = FullscreenBlitProgram();
  SetShader(prog);
  ResetTextures();
  eTextureUnit unit = BindTexture(texture);
  iShaderAttribute* attrib = prog->GetShaderAttribute("Diffuse");
  if (attrib)
  {
    attrib->Bind(unit);
  }
  mesh->Render(this, eRP_Forward);
}



void GL4Device::RenderFullscreen(iTexture2DArray* texture, int layer)
{
  iRenderMesh* mesh = FullscreenBlitRenderMesh();
  GL4Program* prog = FullscreenBlitArrayProgram();
  SetShader(prog);
  ResetTextures();
  eTextureUnit unit = BindTexture(texture);
  iShaderAttribute* attrib = prog->GetShaderAttribute("Diffuse");
  if (attrib)
  {
    attrib->Bind(unit);
  }
  attrib = prog->GetShaderAttribute("ArrayIndex");
  if (attrib)
  {
    attrib->Bind((float)layer);
  }
  mesh->Render(this, eRP_Forward);
}

void GL4Device::RenderFullscreen(iTextureCube* texture, int layer)
{
  iRenderMesh* mesh = FullscreenBlitCubeRenderMesh(layer);
  GL4Program* prog = FullscreenBlitCubeProgram();
  SetShader(prog);
  ResetTextures();
  eTextureUnit unit = BindTexture(texture);
  iShaderAttribute* attrib = prog->GetShaderAttribute("Diffuse");
  if (attrib)
  {
    attrib->Bind(unit);
  }
  mesh->Render(this, eRP_Forward);
}




void GL4Device::BindForwardLight(const iLight* light, Size idx)
{
  if (!m_shader || idx >= SPC_MAX_LIGHTS)
  {
    return;
  }

  SPC_GL_ERROR();

  iShaderAttribute* lightColor = m_shader->GetShaderAttribute(eSA_LightColor);
  iShaderAttribute* lightVector = m_shader->GetShaderAttribute(eSA_LightVector);
  iShaderAttribute* lightRange = m_shader->GetShaderAttribute(eSA_LightRange);
  iShaderAttribute* lightCastShadow = m_shader->GetShaderAttribute(eSA_LightCastShadow);
  SPC_GL_ERROR();

  iShaderAttribute* pointLightShadowMapMappingBias = m_shader->GetShaderAttribute(eSA_PointLightShadowMapMappingBias);
  iShaderAttribute* pointLightShadowMapColor = m_shader->GetShaderAttribute(eSA_PointLightShadowMapColor);
  iShaderAttribute* pointLightShadowMapDepth = m_shader->GetShaderAttribute(eSA_PointLightShadowMapDepth);
  SPC_GL_ERROR();


  iShaderAttribute* directionalLightShadowMapLayersBias = m_shader->GetShaderAttribute(eSA_DirectionalLightShadowMapLayersBias);
  iShaderAttribute* directionalLightShadowMapMatrices = m_shader->GetShaderAttribute(eSA_DirectionalLightShadowMapMatrices);
  iShaderAttribute* directionalLightShadowMapColor = m_shader->GetShaderAttribute(eSA_DirectionalLightShadowMapColor);
  iShaderAttribute* directionalLightShadowMapDepth = m_shader->GetShaderAttribute(eSA_DirectionalLightShadowMapDepth);
  SPC_GL_ERROR();

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
  if (pointLightShadowMapMappingBias)
  {
    pointLightShadowMapMappingBias->SetArrayIndex(idx);
  }
  if (pointLightShadowMapColor)
  {
    pointLightShadowMapColor->SetArrayIndex(idx);
  }
  if (pointLightShadowMapDepth)
  {
    pointLightShadowMapDepth->SetArrayIndex(idx);
  }

  if (directionalLightShadowMapLayersBias)
  {
    directionalLightShadowMapLayersBias->SetArrayIndex(idx);
  }
  if (directionalLightShadowMapMatrices)
  {
    directionalLightShadowMapMatrices->SetArrayIndex(idx * 3);
  }
  if (directionalLightShadowMapColor)
  {
    directionalLightShadowMapColor->SetArrayIndex(idx);
  }
  if (directionalLightShadowMapDepth)
  {
    directionalLightShadowMapDepth->SetArrayIndex(idx);
  }
  SPC_GL_ERROR();


  if (light)
  {
    if (lightColor)
    {
      lightColor->Bind(light->GetColor());
    }

    switch (light->GetType())
    {
    case eLT_Point:
      {
        SPC_GL_ERROR();
        auto pointLight = static_cast<const iPointLight*>(light);
        if (lightVector)
        {
          lightVector->Bind(Vector4f(pointLight->GetPosition(), 1.0f));
        }
        SPC_GL_ERROR();
        if (lightRange)
        {
          lightRange->Bind(pointLight->GetRange());
        }
        if (directionalLightShadowMapDepth)
        {
          directionalLightShadowMapDepth->Bind(0);
        }

        //
        // Bind the shadow mapping.
        auto it = m_pointLightShadowData.find(pointLight);
        if (it != m_pointLightShadowData.end())
        {
          SPC_GL_ERROR();
          PointLightShadowData& data = it->second;
          if (lightCastShadow)
          {
            lightCastShadow->Bind(1);
          }
          SPC_GL_ERROR();
          if (pointLightShadowMapMappingBias)
          {
            pointLightShadowMapMappingBias->Bind(data.Mapping);
          }
          SPC_GL_ERROR();
          if (pointLightShadowMapColor && data.Color)
          {
            eTextureUnit unit = BindTexture(data.Color);
            if (unit != eTU_Invalid)
            {
              pointLightShadowMapColor->Bind(unit);
            }
          }
          SPC_GL_ERROR();
          if (pointLightShadowMapDepth)
          {
            if (data.Depth)
            {
              eTextureUnit unit = BindTexture(data.Depth);
              if (unit != eTU_Invalid)
              {
                pointLightShadowMapDepth->Bind(unit);
              }
            }
            else
            {
              pointLightShadowMapDepth->Bind(0);
            }
          }
          SPC_GL_ERROR();
        }
        else
        {
          if (lightCastShadow)
          {
            lightCastShadow->Bind(0);
          }
          if (pointLightShadowMapDepth)
          {
            pointLightShadowMapDepth->Bind(0);
          }
        }
      }
      break;
    case eLT_Directional:
      {
        auto directionalLight = static_cast<const iDirectionalLight*>(light);
        if (lightVector)
        {
          lightVector->Bind(Vector4f(-directionalLight->GetDirection(), 0.0f));
        }
        if (pointLightShadowMapDepth)
        {
          pointLightShadowMapDepth->Bind(0);
        }


        auto it = m_directionalLightShadowData.find(directionalLight);
        if (it != m_directionalLightShadowData.end())
        {
          DirectionalLightShadowData& data = it->second;
          if (lightCastShadow)
          {
            lightCastShadow->Bind(1);
          }

          if (directionalLightShadowMapLayersBias)
          {
            directionalLightShadowMapLayersBias->Bind(data.LayersBias);
          }
          if (directionalLightShadowMapMatrices)
          {
            directionalLightShadowMapMatrices->Bind(data.Matrices, 3);
          }
          if (directionalLightShadowMapColor && data.Color)
          {
            eTextureUnit unit = BindTexture(data.Color);
            if (unit != eTU_Invalid)
            {
              directionalLightShadowMapColor->Bind(unit);
            }
          }
          if (directionalLightShadowMapDepth)
          {
            if (data.Depth)
            {
              eTextureUnit unit = BindTexture(data.Depth);
              //printf("    Bind directional light shadow map: %p -> %d\n", data.Depth, unit);
              if (unit != eTU_Invalid)
              {
                directionalLightShadowMapDepth->Bind(unit);
              }
            }
            else
            {
              directionalLightShadowMapDepth->Bind(0);
            }
          }
        }
        else
        {
          if (lightCastShadow)
          {
            lightCastShadow->Bind(0);
          }
          if (directionalLightShadowMapDepth)
          {
            directionalLightShadowMapDepth->Bind(0);
          }
        }
      }
      break;
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
    if (pointLightShadowMapDepth)
    {
      pointLightShadowMapDepth->Bind(0);
    }
    if (directionalLightShadowMapDepth)
    {
      directionalLightShadowMapDepth->Bind(0);
    }
  }
  SPC_GL_ERROR();
}

void GL4Device::FinishForwardLights(Size numLights)
{
  SPC_GL_ERROR();
  if (m_shader)
  {
    iShaderAttribute* count = m_shader->GetShaderAttribute(eSA_LightCount);
    if (count)
    {
      count->Bind((int)numLights);
    }
  }
  SPC_GL_ERROR();
}

void GL4Device::BindMatrices()
{
  if (!m_shader)
  {
    return;
  }

  iShaderAttribute* attr = m_shader->GetShaderAttribute(eSA_ModelMatrix);
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
}


void GL4Device::UpdateModelViewMatrix()
{
  m_modelViewMatrix = m_viewMatrix * m_modelMatrix;
  m_modelViewMatrixDirty = false;
}


void GL4Device::UpdateViewProjectionMatrix()
{
  m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
  m_viewProjectionMatrixDirty = false;
}

void GL4Device::UpdateModelViewProjectionMatrix()
{
  if (m_modelViewMatrixDirty)
  {
    UpdateModelViewMatrix();
  }
  m_modelViewProjectionMatrix = m_projectionMatrix * m_modelViewMatrix;
  m_modelViewProjectionMatrixDirty = false;
}

void GL4Device::UpdateModelMatrixInv()
{
  m_modelMatrixInv = m_modelMatrix.Inverted();
  m_modelMatrixInvDirty = false;
}


void GL4Device::UpdateViewMatrixInv()
{
  m_viewMatrixInv = m_viewMatrix.Inverted();
  m_viewMatrixInvDirty = false;
}

void GL4Device::UpdateProjectionMatrixInv()
{
  m_projectionMatrixInv = m_projectionMatrix.Inverted();
  m_projectionMatrixInvDirty = false;
}


void GL4Device::UpdateModelViewMatrixInv()
{
  if (m_modelViewMatrixDirty)
  {
    UpdateModelViewMatrix();
  }
  m_modelViewMatrixInv = m_modelViewMatrix.Inverted();
  m_modelViewMatrixInvDirty = false;
}


void GL4Device::UpdateViewProjectionMatrixInv()
{
  if (m_viewProjectionMatrixDirty)
  {
    UpdateViewProjectionMatrix();
  }
  m_viewProjectionMatrixInv = m_viewProjectionMatrix.Inverted();
  m_viewProjectionMatrixInvDirty = false;
}

void GL4Device::UpdateModelViewProjectionMatrixInv()
{
  if (m_modelViewProjectionMatrixDirty)
  {
    UpdateModelViewProjectionMatrix();
  }
  m_modelViewProjectionMatrixInv = m_modelViewProjectionMatrix.Inverted();
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


GL4Program* GL4Device::FullscreenBlitProgram()
{
  if (!m_fullscreenBlitProgram)
  {
    m_fullscreenBlitProgram = AssetManager::Get()->Load<GL4Program>("file:///engine/opengl/gl4/fullscreen_blit.spc");
  }
  return m_fullscreenBlitProgram;
}


GL4Program* GL4Device::FullscreenBlitArrayProgram()
{
  if (!m_fullscreenBlitArrayProgram)
  {
    m_fullscreenBlitArrayProgram = AssetManager::Get()->Load<GL4Program>("file:///engine/opengl/gl4/fullscreen_blit_array.spc");
  }
  return m_fullscreenBlitArrayProgram;
}


iRenderMesh* GL4Device::FullscreenBlitRenderMesh()
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
    std::vector<UInt32> indices;
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


GL4Program* GL4Device::FullscreenBlitCubeProgram()
{
  if (!m_fullscreenBlitCubeProgram)
  {
    m_fullscreenBlitCubeProgram = AssetManager::Get()->Load<GL4Program>("file:///engine/opengl/gl4/fullscreen_blit_cube.spc");
  }
  return m_fullscreenBlitCubeProgram;
}

iRenderMesh* GL4Device::FullscreenBlitCubeRenderMesh(int layer)
{
  switch (layer) {
  case 0: if (m_fullscreenBlitCubePosXRenderMesh) return m_fullscreenBlitCubePosXRenderMesh; else break;
  case 1: if (m_fullscreenBlitCubeNegXRenderMesh) return m_fullscreenBlitCubeNegXRenderMesh; else break;
  case 2: if (m_fullscreenBlitCubePosYRenderMesh) return m_fullscreenBlitCubePosYRenderMesh; else break;
  case 3: if (m_fullscreenBlitCubeNegYRenderMesh) return m_fullscreenBlitCubeNegYRenderMesh; else break;
  case 4: if (m_fullscreenBlitCubePosZRenderMesh) return m_fullscreenBlitCubePosZRenderMesh; else break;
  case 5: if (m_fullscreenBlitCubeNegZRenderMesh) return m_fullscreenBlitCubeNegZRenderMesh; else break;
  default: break;
  }


  GL4RenderMeshGenerator gen;
  std::vector<Vector4f> vertices4;
  vertices4.push_back(Vector4f(-1.0f, -1.0f, 0.0f, 1.0f));
  vertices4.push_back(Vector4f(-1.0f, 1.0f, 0.0f, 1.0f));
  vertices4.push_back(Vector4f(1.0f, -1.0f, 0.0f, 1.0f));
  vertices4.push_back(Vector4f(1.0f, 1.0f, 0.0f, 1.0f));
  std::vector<UInt32> indices;
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
  m_numDrawCalls = 0;
  m_numTriangles = 0;
}

Size GL4Device::GetNumberOfDrawCalls() const
{
  return m_numDrawCalls;
}

Size GL4Device::GetNumberOfTriangles() const
{
  return m_numTriangles;
}

#endif

}