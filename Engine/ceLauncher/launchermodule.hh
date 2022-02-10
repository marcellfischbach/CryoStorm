
#pragma once

namespace ce
{


class LauncherModule
{
public:
  static bool Register(int argc, char** argv);
  static bool Initialize(int argc, char** argv);

private:
  LauncherModule() { }
};

}