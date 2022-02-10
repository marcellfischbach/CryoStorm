
#pragma once

#include <ceOpenGL/openglexport.hh>

namespace ce::opengl
{

class CE_OGL_API OpenGLModule
{
public:
  static bool Register(int argc, char** argv);
  static bool Initialize(int argc, char** argv);

private:
  OpenGLModule() { }
};

}