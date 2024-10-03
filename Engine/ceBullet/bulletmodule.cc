
#include <ceBullet/bulletmodule.hh>
#include <ceBullet/bulletsystem.hh>
#include <master.refl.cc>
#include <ceCore/objectregistry.hh>



namespace ce::bullet
{

bool BulletModule::Register(const std::vector<std::string> &args, Engine *engine)
{
  register_classes();


  ObjectRegistry::Register<iPhysicsSystem>(new BulletSystem());

  return true;
}

bool BulletModule::Initialize(const std::vector<std::string> &args, Engine *engine)
{
  return true;
}


}

CS_DEFINE_LIBRARY(ceBullet)
{
  printf ("Load library: ceBullet\n");
  return new ce::bullet::BulletModule;
}