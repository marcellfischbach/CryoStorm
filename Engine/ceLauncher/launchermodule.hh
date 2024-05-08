
#pragma once

#include <ceCore/imodule.hh>


namespace ce::launcher
{

class LauncherModule : public ce::iModule
{
public:
  bool Register(int argc, char** argv, ce::Engine* engine);
  bool Initialize(int argc, char** argv, ce::Engine* engine);

};

}
