
#include <spcOpenGL/gl4/gl4graphics.hh>
#include <GL/glew.h>

namespace spc
{

GL4Graphics::GL4Graphics()
{
  SPC_CLASS_GEN_CONSTR;
}

GL4Graphics::~GL4Graphics()
{

}

bool GL4Graphics::Initialize()
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

  return true;
}

void GL4Graphics::SetViewport(Int16 x, Int16 y, UInt16 width, UInt16 height)
{
  glViewport(x, y, width, height);
}

void GL4Graphics::Clear(bool clearColor, const Color4f& color, bool clearDepth, float depth, bool clearStencil, UInt8 stencil)
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


}