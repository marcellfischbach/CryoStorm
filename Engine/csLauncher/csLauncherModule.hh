
#pragma once

#include <csCore/iModule.hh>


namespace cs::launcher
{

class csLauncherModule : public cs::iModule
{
public:
  bool Register(const std::vector<std::string> &args, csEngine *engine);
  bool Initialize(const std::vector<std::string> &args, csEngine *engine);

};

}
