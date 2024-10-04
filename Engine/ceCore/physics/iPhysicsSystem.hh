
#pragma once

#include <ceCore/csCoreExport.hh>
#include <ceCore/csClass.hh>

namespace cryo
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


CS_CLASS()
struct CS_CORE_API iPhysicsSystem : public CS_SUPER(iObject)
{
  CS_CLASS_GEN;

  ~iPhysicsSystem() override = default;


  CS_NODISCARD virtual iPhysicsWorld *CreateWorld() = 0;

  CS_NODISCARD virtual iCollisionShape* CreateShape(const SphereShapeDesc & desc) = 0;
  CS_NODISCARD virtual iCollisionShape* CreateShape(const BoxShapeDesc & desc) = 0;
  CS_NODISCARD virtual iCollisionShape* CreateShape(const CylinderShapeDesc & desc) = 0;
  CS_NODISCARD virtual iCollisionShape* CreateShape(const CapsuleShapeDesc & desc) = 0;


  CS_NODISCARD virtual iStaticCollider* CreateStaticCollider() = 0;
  CS_NODISCARD virtual iDynamicCollider* CreateDynamicCollider() = 0;
  CS_NODISCARD virtual iTriggerCollider* CreateTriggerCollider() = 0;

};


}