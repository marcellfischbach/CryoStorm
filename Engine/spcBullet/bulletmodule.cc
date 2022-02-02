
#include <spcBullet/bulletmodule.hh>
#include <master.refl.cc>


namespace spc::bullet
{

bool BulletModule::Register(int argc, char** argv)
{
  register_classes();

  return true;
}

bool BulletModule::Initialize(int argc, char** argv)
{
  return true;
}


}