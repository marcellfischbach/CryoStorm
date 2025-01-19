
#pragma once

#include <csBullet/csBulletExport.hh>
#include <csCore/iModule.hh>

namespace cs::bullet
{

CS_CLASS()
class CS_BULLET_API csBulletModule : public CS_SUPER(iModule)
{
CS_CLASS_GEN_OBJECT;
public:
  bool Register(const std::vector<std::string> &args, csEngine *engine);
  bool Initialize(const std::vector<std::string> &args, csEngine *engine);
  void Shutdown(const std::vector<std::string> &args, csEngine *engine);
};

}

CS_DECLARE_LIBRARY(csBullet);