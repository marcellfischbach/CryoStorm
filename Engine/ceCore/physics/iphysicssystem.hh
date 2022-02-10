
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>

namespace ce
{

struct iPhysicsWorld;
struct SphereShapeDesc;
struct BoxShapeDesc;
struct CylinderShapeDesc;
struct CapsuleShapeDesc;
struct iCollisionShape;
struct iStaticCollider;
struct iDynamicCollider;
struct iTriggerCollider;


CE_CLASS()
struct CE_CORE_API iPhysicsSystem : public CE_SUPER(iObject)
{
  CE_CLASS_GEN;

  ~iPhysicsSystem() override = default;


  CE_NODISCARD virtual iPhysicsWorld *CreateWorld() = 0;

  CE_NODISCARD virtual iCollisionShape* CreateShape(const SphereShapeDesc & desc) = 0;
  CE_NODISCARD virtual iCollisionShape* CreateShape(const BoxShapeDesc & desc) = 0;
  CE_NODISCARD virtual iCollisionShape* CreateShape(const CylinderShapeDesc & desc) = 0;
  CE_NODISCARD virtual iCollisionShape* CreateShape(const CapsuleShapeDesc & desc) = 0;


  CE_NODISCARD virtual iStaticCollider* CreateStaticCollider() = 0;
  CE_NODISCARD virtual iDynamicCollider* CreateDynamicCollider() = 0;
  CE_NODISCARD virtual iTriggerCollider* CreateTriggerCollider() = 0;

};


}