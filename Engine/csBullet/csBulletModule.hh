
#pragma once

#include <csBullet/csBulletExport.hh>
#include <csCore/iModule.hh>

namespace cs::bullet
{

class CS_BULLET_API csBulletModule : public iModule
{
public:
  bool Register(const std::vector<std::string> &args, csEngine *engine);
  bool Initialize(const std::vector<std::string> &args, csEngine *engine);

};

}

CS_DECLARE_LIBRARY(csBullet);