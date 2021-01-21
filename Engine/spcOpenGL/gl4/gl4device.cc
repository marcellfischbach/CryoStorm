
#include <spcOpenGL/gl4/gl4device.hh>
#include <spcOpenGL/gl4/gl4directionallight.hh>
#include <spcOpenGL/gl4/gl4pointlight.hh>
#include <spcOpenGL/gl4/gl4sampler.hh>
#include <spcOpenGL/gl4/gl4texture2d.hh>
#include <spcOpenGL/gl4/shading/gl4program.hh>
#include <spcCore/graphics/shading/ishaderattribute.hh>
#include <GL/glew.h>
#include <iostream>

#define SPC_MAX_LIGHTS 4

namespace spc::opengl
{

GL4Device::GL4Device()
  : iDevice()
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

  printf("OpenGL capabilities:\n");
  printf("  Vendor  : %s\n", (const char*)glGetString(GL_VENDOR));
  printf("  Renderer: %s\n", (const char*)glGetString(GL_RENDERER));
  printf("  Version : %s\n", (const char*)glGetString(GL_VERSION));
  printf("  GLSL    : %s\n", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

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

  return true;
}

void GL4Device::SetViewport(Int16 x, Int16 y, UInt16 width, UInt16 height)
{
  glViewport(x, y, width, height);
}

void GL4Device::Clear(bool clearColor, const Color4f& color, bool clearDepth, float depth, bool clearStencil, UInt8 stencil)
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

Matrix4f& GL4Device::GetPerspectiveProjection(float l, float r, float b, float t, float n, float f, Matrix4f& m)
{
  float z2 = 2.0f * n;
  float dx = r - l;
  float dy = t - b;
  float dz = f - n;
  float sx = r + l;
  float sy = t + b;
  float sz = n + f;


  m.m00 = z2 / dx; m.m10 = 0.0f;    m.m20 = -sx / dx;   m.m30 = 0.0f;
  m.m01 = 0.0f;    m.m11 = z2 / dy; m.m21 = -sy / dy;   m.m31 = 0.0f;
  m.m02 = 0.0f;    m.m12 = 0.0f;    m.m22 = sz / dz;    m.m32 = -2.0 * n * f / dz;
  m.m03 = 0.0f;    m.m13 = 0.0f;    m.m23 = 1.0f;       m.m33 = 0.0f;

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


  m.m00 = dx / z2; m.m10 = 0.0f;    m.m20 = 0.0f;      m.m30 = sx / z2;
  m.m01 = 0.0f;    m.m11 = dy / z2; m.m21 = 0.0f;      m.m31 = sy / z2;
  m.m02 = 0.0f;    m.m12 = 0.0f;    m.m22 = 0.0f;      m.m32 = 1.0f;
  m.m03 = 0.0f;    m.m13 = 0.0f;    m.m23 = -dz / nf2;  m.m33 = sz / nf2;


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

  m.m00 = 2.0f / dx; m.m10 = 0.0f;      m.m20 = 0.0f;      m.m30 = -sx / dx;
  m.m01 = 0.0f;      m.m11 = 2.0f / dy; m.m21 = 0.0f;      m.m31 = -sy / dy;
  m.m02 = 0.0f;      m.m12 = 0.0f;      m.m22 = 2.0f / dz; m.m32 = -sz / dz;
  m.m03 = 0.0f;      m.m13 = 0.0f;      m.m23 = 0.0f;      m.m33 = 1.0;
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

  m.m00 = dx / 2.0f; m.m10 = 0.0f;      m.m20 = 0.0f;       m.m30 = sx / 2.0f;
  m.m01 = 0.0f;      m.m11 = dy / 2.0f; m.m21 = 0.0f;       m.m31 = sy / 2.0f;
  m.m02 = 0.0f;      m.m12 = 0.0f;      m.m22 = dz / 2.0f;  m.m32 = sz / 2.0f;
  m.m03 = 0.0f;      m.m13 = 0.0f;      m.m23 = 0.0f;       m.m33 = 1.0;

  return m;
}


void GL4Device::SetShader(iShader* shader)
{
  if (shader == m_shader)
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
  return texture;
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
    BindMatrices();
    mesh->Render(this, pass);
  }
}



void GL4Device::BindForwardLight(iLight* light, Size idx)
{
  if (!m_shader || idx >= SPC_MAX_LIGHTS)
  {
    return;
  }

  iShaderAttribute* lightColor = m_shader->GetShaderAttribute(eSA_LightColor);
  iShaderAttribute* lightVector = m_shader->GetShaderAttribute(eSA_LightVector);
  iShaderAttribute* lightRange = m_shader->GetShaderAttribute(eSA_LightRange);
  if (lightColor) lightColor->SetArrayIndex(idx);
  if (lightVector) lightVector->SetArrayIndex(idx);
  if (lightRange) lightRange->SetArrayIndex(idx);

  if (light)
  {

    if (lightColor) lightColor->Bind(light->GetColor());

    switch (light->GetType())
    {
    case eLT_Point:
    {
      iPointLight* pointLight = static_cast<iPointLight*>(light);
      if (lightVector) lightVector->Bind(Vector4f(pointLight->GetPosition(), 1.0f));
      if (lightRange) lightRange->Bind(pointLight->GetRange());
    }
    break;
    case eLT_Directional:
    {
      iDirectionalLight* directionalLight = static_cast<iDirectionalLight*>(light);
      if (lightVector) lightVector->Bind(Vector4f(directionalLight->GetDirection(), 0.0f));
    }
    break;
    }
  }
  else
  {
    if (lightColor) lightColor->Bind(Color4f(0.0f, 0.0f, 0.0f));
    if (lightVector) lightVector->Bind(Vector4f(0.0f, 0.0f, 0.0f, 0.0f));
  }
}

void GL4Device::FinishForwardLights(Size numLights)
{
  for (Size i = numLights; i < SPC_MAX_LIGHTS; i++)
  {
    BindForwardLight(nullptr, i);
  }
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
}


void GL4Device::UpdateModelViewMatrix()
{
  Matrix4f::Mult(m_viewMatrix, m_modelMatrix, m_modelViewMatrix);
  m_modelViewMatrixDirty = false;
}


void GL4Device::UpdateViewProjectionMatrix()
{
  Matrix4f::Mult(m_projectionMatrix, m_viewMatrix, m_viewProjectionMatrix);
  m_viewProjectionMatrixDirty = false;
}

void GL4Device::UpdateModelViewProjectionMatrix()
{
  if (m_modelViewMatrixDirty)
  {
    UpdateModelViewMatrix();
  }
  Matrix4f::Mult(m_projectionMatrix, m_modelViewMatrix, m_modelViewProjectionMatrix);
  m_modelViewProjectionMatrixDirty = false;
}

void GL4Device::UpdateModelMatrixInv()
{
  m_modelMatrix.Inverted(m_modelMatrixInv);
  m_modelMatrixInvDirty = false;
}


void GL4Device::UpdateViewMatrixInv()
{
  m_viewMatrix.Inverted(m_viewMatrixInv);
  m_viewMatrixInvDirty = false;
}

void GL4Device::UpdateProjectionMatrixInv()
{
  m_projectionMatrix.Inverted(m_projectionMatrixInv);
  m_projectionMatrixInvDirty = false;
}


void GL4Device::UpdateModelViewMatrixInv()
{
  if (m_modelViewMatrixDirty)
  {
    UpdateModelViewMatrix();
  }
  m_modelViewMatrix.Inverted(m_modelViewMatrixInv);
  m_modelViewMatrixInvDirty = false;
}


void GL4Device::UpdateViewProjectionMatrixInv()
{
  if (m_viewProjectionMatrixDirty)
  {
    UpdateViewProjectionMatrix();
  }
  m_viewProjectionMatrix.Inverted(m_viewProjectionMatrixInv);
  m_viewProjectionMatrixInvDirty = false;
}

void GL4Device::UpdateModelViewProjectionMatrixInv()
{
  if (m_modelViewProjectionMatrixDirty)
  {
    UpdateModelViewProjectionMatrix();
  }
  m_modelViewProjectionMatrix.Inverted(m_modelViewProjectionMatrixInv);
  m_modelViewProjectionMatrixInvDirty = false;
}

}