
#pragma once

#include <ceBullet/bulletexport.hh>
#include <ceCore/physics/iphysicssystem.hh>

namespace ce::bullet
{

CE_CLASS()

class CE_BULLET_API BulletSystem : public CE_SUPER(iPhysicsSystem)
{
CE_CLASS_GEN_OBJECT;
public:
  BulletSystem();

  ~BulletSystem() override;

  CE_NODISCARD iPhysicsWorld *CreateWorld() override;


  CE_NODISCARD iCollisionShape* CreateShape(const SphereShapeDesc & desc) override;
  CE_NODISCARD iCollisionShape* CreateShape(const BoxShapeDesc & desc) override;
  CE_NODISCARD iCollisionShape* CreateShape(const CylinderShapeDesc & desc) override;
  CE_NODISCARD iCollisionShape* CreateShape(const CapsuleShapeDesc & desc) override;


  CE_NODISCARD iStaticCollider* CreateStaticCollider() override;
  CE_NODISCARD iDynamicCollider* CreateDynamicCollider() override;
  CE_NODISCARD iTriggerCollider* CreateTriggerCollider() override;

};


}
