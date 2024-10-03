
#pragma once

#include <ceBullet/bulletexport.hh>
#include <ceCore/physics/iphysicssystem.hh>

namespace ce::bullet
{

CS_CLASS()

class CS_BULLET_API BulletSystem : public CS_SUPER(iPhysicsSystem)
{
CS_CLASS_GEN_OBJECT;
public:
  BulletSystem();

  ~BulletSystem() override;

  CS_NODISCARD iPhysicsWorld *CreateWorld() override;


  CS_NODISCARD iCollisionShape* CreateShape(const SphereShapeDesc & desc) override;
  CS_NODISCARD iCollisionShape* CreateShape(const BoxShapeDesc & desc) override;
  CS_NODISCARD iCollisionShape* CreateShape(const CylinderShapeDesc & desc) override;
  CS_NODISCARD iCollisionShape* CreateShape(const CapsuleShapeDesc & desc) override;


  CS_NODISCARD iStaticCollider* CreateStaticCollider() override;
  CS_NODISCARD iDynamicCollider* CreateDynamicCollider() override;
  CS_NODISCARD iTriggerCollider* CreateTriggerCollider() override;

};


}
