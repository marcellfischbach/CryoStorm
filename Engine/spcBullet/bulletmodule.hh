
#pragma once

#include <spcBullet/bulletexport.hh>

namespace spc::bullet
{

class SPC_BULLET_API BulletModule
{
public:
  static bool Register(int argc, char** argv);
  static bool Initialize(int argc, char** argv);

private:
  BulletModule() { }
};

}