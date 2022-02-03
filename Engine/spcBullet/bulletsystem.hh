
#pragma once

#include <spcBullet/bulletexport.hh>
#include <spcCore/physics/iphysicssystem.hh>

namespace spc::bullet
{

SPC_CLASS()

class SPC_BULLET_API BulletSystem : public SPC_SUPER(iPhysicsSystem)
{
SPC_CLASS_GEN_OBJECT;
public:
  BulletSystem();

  ~BulletSystem() override;

  iPhysicsWorld *CreateWorld() const override;

};


}
