
#pragma once

#include <ceBullet/bulletexport.hh>

namespace ce::bullet
{

class CE_BULLET_API BulletModule
{
public:
  static bool Register(int argc, char** argv);
  static bool Initialize(int argc, char** argv);

private:
  BulletModule() { }
};

}