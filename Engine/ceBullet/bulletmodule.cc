
#include <ceBullet/bulletmodule.hh>
#include <ceBullet/bulletsystem.hh>
#include <master.refl.cc>
#include <ceCore/objectregistry.hh>



namespace ce::bullet
{

bool BulletModule::Register(int argc, char** argv)
{
  register_classes();


  ObjectRegistry::Register<iPhysicsSystem>(new BulletSystem());

  return true;
}

bool BulletModule::Initialize(int argc, char** argv)
{
  return true;
}


}