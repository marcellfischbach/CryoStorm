
#include <csBullet/csBulletModule.hh>
#include <csBullet/csBulletSystem.hh>
#include <master.refl.cc>
#include <csCore/csObjectRegistry.hh>



namespace cs::bullet
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

CS_DEFINE_LIBRARY(csBullet)
{
  return new cs::bullet::csBulletModule;
}