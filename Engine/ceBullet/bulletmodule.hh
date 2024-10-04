
#pragma once

#include <ceBullet/bulletexport.hh>
#include <ceCore/iModule.hh>

namespace cryo::bullet
{

class CS_BULLET_API BulletModule : public iModule
{
public:
  bool Register(const std::vector<std::string> &args, csEngine *engine);
  bool Initialize(const std::vector<std::string> &args, csEngine *engine);

};

}

CS_DECLARE_LIBRARY(ceBullet);