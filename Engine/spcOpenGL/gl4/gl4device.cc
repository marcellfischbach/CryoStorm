
#include <spcOpenGL/gl4/gl4device.hh>
#include <spcOpenGL/gl4/shading/gl4program.hh>
#include <spcOpenGL/gl4/gl4texture2d.hh>
#include <GL/glew.h>

namespace spc
{

GL4Device::GL4Device()
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

void GL4Device::SetShader(iShader* shader)
{
  if (shader)
  {
    GL4Program* program = static_cast<GL4Program*>(shader);

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
  if (m_nextTextureUnit == eTU_Invalid)
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


}