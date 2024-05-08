
#pragma once

#include <ceCore/imodule.hh>


namespace ce::launcher
{

class LauncherModule : public ce::iModule
{
public:
  bool Register(const std::vector<std::string> &args, Engine *engine);
  bool Initialize(const std::vector<std::string> &args, Engine *engine);

};

}
