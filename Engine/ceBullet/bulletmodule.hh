
#pragma once

#include <ceBullet/bulletexport.hh>
#include <ceCore/imodule.hh>

namespace ce::bullet
{

class CE_BULLET_API BulletModule : public iModule
{
public:
  bool Register(const std::vector<std::string> &args, Engine *engine);
  bool Initialize(const std::vector<std::string> &args, Engine *engine);

};

}

CE_DECLARE_LIBRARY(ceBullet);