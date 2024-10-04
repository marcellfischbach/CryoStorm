
#pragma once

#include <csCore/iModule.hh>


namespace cryo::launcher
{

class LauncherModule : public cryo::iModule
{
public:
  bool Register(const std::vector<std::string> &args, csEngine *engine);
  bool Initialize(const std::vector<std::string> &args, csEngine *engine);

};

}
