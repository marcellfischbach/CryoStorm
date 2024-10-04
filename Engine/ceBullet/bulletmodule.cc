
#include <ceBullet/bulletmodule.hh>
#include <ceBullet/bulletsystem.hh>
#include <master.refl.cc>
#include <ceCore/csObjectRegistry.hh>



namespace cryo::bullet
{

bool BulletModule::Register(const std::vector<std::string> &args, csEngine *engine)
{
  register_classes();


  csObjectRegistry::Register<iPhysicsSystem>(new BulletSystem());

  return true;
}

bool BulletModule::Initialize(const std::vector<std::string> &args, csEngine *engine)
{
  return true;
}


}

CS_DEFINE_LIBRARY(ceBullet)
{
  printf ("Load library: ceBullet\n");
  return new cryo::bullet::BulletModule;
}