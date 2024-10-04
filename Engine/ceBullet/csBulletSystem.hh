
#pragma once

#include <ceBullet/csBulletExport.hh>
#include <ceCore/physics/iPhysicsSystem.hh>

namespace cryo::bullet
{

CS_CLASS()

class CS_BULLET_API csBulletSystem : public CS_SUPER(iPhysicsSystem)
{
CS_CLASS_GEN_OBJECT;
public:
  csBulletSystem();

  ~csBulletSystem() override;

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
