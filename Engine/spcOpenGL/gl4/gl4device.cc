
#include <spcOpenGL/gl4/gl4device.hh>
#include <spcOpenGL/gl4/shading/gl4program.hh>
#include <spcOpenGL/gl4/gl4texture2d.hh>
#include <spcCore/graphics/shading/ishaderattribute.hh>
#include <GL/glew.h>

namespace spc
{

GL4Device::GL4Device()
  : iDevice()
  , m_shader(nullptr)
  , m_modelViewMatrixDirty (true)
  , m_viewProjectionMatrixDirty (true)
  , m_modelViewProjectionMatrixDirty(true)
  , m_modelMatrixInvDirty (true)
  , m_viewMatrixInvDirty (true)
  , m_projectionMatrixInvDirty(true)
  , m_modelViewMatrixInvDirty(true)
  , m_viewProjectionMatrixInvDirty(true)
  , m_modelViewProjectionMatrixInvDirty(true)
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

void GL4Device::SetModelMatrix(const Matrix4f &modelMatrix)
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


void GL4Device::SetModelMatrix(const Matrix4f& modelMatrix, const Matrix4f &modelMatrixInv)
{
  m_modelMatrix = modelMatrix;
  m_modelMatrixInv = modelMatrixInv;

  m_modelViewMatrixDirty = true;
  m_modelViewProjectionMatrixDirty = true;

  m_modelViewMatrixInvDirty = true;
  m_modelViewProjectionMatrixInvDirty = true;
}

void GL4Device::SetViewMatrix(const Matrix4f& viewMatrix, const Matrix4f &viewMatrixInv)
{
  m_viewMatrix = viewMatrix;
  m_viewMatrixInv = viewMatrixInv;

  m_modelViewMatrixDirty = true;
  m_viewProjectionMatrixDirty = true;


  m_modelViewMatrixInvDirty = true;
  m_viewProjectionMatrixInvDirty = true;
}

void GL4Device::SetProjectionMatrix(const Matrix4f& projectionMatrix, const Matrix4f &projectionMatrixInv)
{
  m_projectionMatrix = projectionMatrix;
  m_projectionMatrixInv = projectionMatrixInv;

  m_viewProjectionMatrixDirty = true;
  m_modelViewProjectionMatrixDirty = true;

  m_viewProjectionMatrixInvDirty = true;
  m_modelViewProjectionMatrixInvDirty = true;
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

eTextureUnit GL4Device::BindTexture(iTexture* texture)
{
  if (!texture ||m_nextTextureUnit == eTU_Invalid)
  {
    return eTU_Invalid;
  }

  eTextureUnit unit = ShiftTextureUnit();
  glActiveTexture(GL_TEXTURE0 + unit);

  switch (texture->GetType())
  {
  case eTT_Texture2D:
    static_cast<GL4Texture2D*>(texture)->Bind();
    break;
  }


  return unit;
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