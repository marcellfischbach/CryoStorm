
#pragma once

#include <SpiceOpenGL/openglexport.hh>

namespace Spice
{

class SPICE_OGL_API OpenGLModule
{
public:
  static bool Register(int argc, char** argv);
  static bool Initialize(int argc, char** argv);

private:
  OpenGLModule() { }
};

}