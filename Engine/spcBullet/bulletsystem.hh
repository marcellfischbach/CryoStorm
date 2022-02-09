
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

  SPC_NODISCARD iPhysicsWorld *CreateWorld() override;


  SPC_NODISCARD iCollisionShape* CreateShape(const SphereShapeDesc & desc) override;
  SPC_NODISCARD iCollisionShape* CreateShape(const BoxShapeDesc & desc) override;
  SPC_NODISCARD iCollisionShape* CreateShape(const CylinderShapeDesc & desc) override;
  SPC_NODISCARD iCollisionShape* CreateShape(const CapsuleShapeDesc & desc) override;


  SPC_NODISCARD iStaticCollider* CreateStaticCollider() override;
  SPC_NODISCARD iDynamicCollider* CreateDynamicCollider() override;
  SPC_NODISCARD iTriggerCollider* CreateTriggerCollider() override;

};


}
