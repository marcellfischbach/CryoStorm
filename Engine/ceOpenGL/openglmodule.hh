
#pragma once

#include <ceOpenGL/openglexport.hh>
#include <ceCore/imodule.hh>

namespace ce::opengl
{

class CE_OGL_API OpenGLModule : public iModule
{
public:
  bool Register(int argc, char** argv, Engine* engine) override;
  bool Initialize(int argc, char** argv, Engine *engine) override;

};

}