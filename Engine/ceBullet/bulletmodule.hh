
#pragma once

#include <ceBullet/bulletexport.hh>
#include <ceCore/imodule.hh>

namespace ce::bullet
{

class CE_BULLET_API BulletModule : public iModule
{
public:
  bool Register(int argc, char** argv, Engine* engine);
  bool Initialize(int argc, char** argv, Engine* engine);

};

}

CE_DECLARE_LIBRARY(ceBullet);