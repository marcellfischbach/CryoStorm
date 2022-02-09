
#include <spcBullet/bulletmodule.hh>
#include <spcBullet/bulletsystem.hh>
#include <master.refl.cc>
#include <spcCore/objectregistry.hh>



namespace spc::bullet
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