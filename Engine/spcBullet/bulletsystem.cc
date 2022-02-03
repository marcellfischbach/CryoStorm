#include <spcBullet/bulletsystem.hh>
#include <spcBullet/bulletworld.hh>


namespace spc::bullet
{

BulletSystem::BulletSystem()
{
  SPC_CLASS_GEN_CONSTR;
}

BulletSystem::~BulletSystem()
{

}


iPhysicsWorld *BulletSystem::CreateWorld() const
{
  return new BulletWorld();
}

}