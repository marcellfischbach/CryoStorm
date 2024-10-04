
#include <ceBullet/csBulletModule.hh>
#include <ceBullet/csBulletSystem.hh>
#include <master.refl.cc>
#include <ceCore/csObjectRegistry.hh>



namespace cryo::bullet
{

bool csBulletModule::Register(const std::vector<std::string> &args, csEngine *engine)
{
  register_classes();


  csObjectRegistry::Register<iPhysicsSystem>(new csBulletSystem());

  return true;
}

bool csBulletModule::Initialize(const std::vector<std::string> &args, csEngine *engine)
{
  return true;
}


}

CS_DEFINE_LIBRARY(ceBullet)
{
  printf ("Load library: ceBullet\n");
  return new cryo::bullet::csBulletModule;
}