
#pragma once

#include <ceCore/imodule.hh>


namespace cryo::launcher
{

class LauncherModule : public cryo::iModule
{
public:
  bool Register(const std::vector<std::string> &args, Engine *engine);
  bool Initialize(const std::vector<std::string> &args, Engine *engine);

};

}
