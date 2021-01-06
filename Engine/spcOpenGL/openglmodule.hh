
#pragma once

#include <spcOpenGL/openglexport.hh>

namespace spc::opengl
{

class SPC_OGL_API OpenGLModule
{
public:
  static bool Register(int argc, char** argv);
  static bool Initialize(int argc, char** argv);

private:
  OpenGLModule() { }
};

}