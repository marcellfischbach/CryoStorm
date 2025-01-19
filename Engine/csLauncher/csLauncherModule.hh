
#pragma once

#include <csCore/iModule.hh>


namespace cs::launcher
{

CS_CLASS()
class csLauncherModule : public CS_SUPER(cs::iModule)
{
CS_CLASS_GEN_OBJECT;
public:
  bool Register(const std::vector<std::string> &args, csEngine *engine);
  bool Initialize(const std::vector<std::string> &args, csEngine *engine);
  void Shutdown(const std::vector<std::string> &args, csEngine *engine);

};

}
