
#pragma once

#include <csBullet/csBulletExport.hh>
#include <csCore/physics/iPhysicsSystem.hh>

namespace cs::bullet
{

CS_CLASS()

class CS_BULLET_API csBulletSystem : public CS_SUPER(iPhysicsSystem)
{
CS_CLASS_GEN_OBJECT;
public:
  csBulletSystem();

  ~csBulletSystem() override;

  CS_NODISCARD csOwned<iPhysicsWorld> CreateWorld() override;


  CS_NODISCARD csOwned<iCollisionShape> CreateShape(const SphereShapeDesc & desc) override;
  CS_NODISCARD csOwned<iCollisionShape> CreateShape(const BoxShapeDesc & desc) override;
  CS_NODISCARD csOwned<iCollisionShape> CreateShape(const CylinderShapeDesc & desc) override;
  CS_NODISCARD csOwned<iCollisionShape> CreateShape(const CapsuleShapeDesc & desc) override;


  CS_NODISCARD csOwned<iStaticCollider> CreateStaticCollider() override;
  CS_NODISCARD csOwned<iDynamicCollider> CreateDynamicCollider() override;
  CS_NODISCARD csOwned<iTriggerCollider> CreateTriggerCollider() override;

};


}
