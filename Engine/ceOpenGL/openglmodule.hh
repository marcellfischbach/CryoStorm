
#pragma once

#include <ceOpenGL/openglexport.hh>
#include <ceCore/iModule.hh>

namespace cryo::opengl
{

class CS_OGL_API OpenGLModule : public iModule
{
public:
  bool Register(const std::vector<std::string> &args, csEngine *engine) override;
  bool Initialize(const std::vector<std::string> &args, csEngine *engine) override;

};

}

CS_DECLARE_LIBRARY(ceOpenGL);