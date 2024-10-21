
#pragma once

#include <csOpenGL/csOpenGLExport.hh>
#include <csCore/iModule.hh>

namespace cs::opengl
{

CS_CLASS()
class CS_OGL_API csOpenGLModule : public CS_SUPER(iModule)
{
CS_CLASS_GEN_OBJECT;
public:
  bool Register(const std::vector<std::string> &args, csEngine *engine) override;
  bool Initialize(const std::vector<std::string> &args, csEngine *engine) override;

};

}

CS_DECLARE_LIBRARY(csOpenGL);