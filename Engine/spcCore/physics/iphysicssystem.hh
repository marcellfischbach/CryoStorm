
#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>

namespace spc
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


SPC_CLASS()
struct SPC_CORE_API iPhysicsSystem : public SPC_SUPER(iObject)
{
  SPC_CLASS_GEN;

  ~iPhysicsSystem() override = default;


  SPC_NODISCARD virtual iPhysicsWorld *CreateWorld() = 0;

  SPC_NODISCARD virtual iCollisionShape* CreateShape(const SphereShapeDesc & desc) = 0;
  SPC_NODISCARD virtual iCollisionShape* CreateShape(const BoxShapeDesc & desc) = 0;
  SPC_NODISCARD virtual iCollisionShape* CreateShape(const CylinderShapeDesc & desc) = 0;
  SPC_NODISCARD virtual iCollisionShape* CreateShape(const CapsuleShapeDesc & desc) = 0;


  SPC_NODISCARD virtual iStaticCollider* CreateStaticCollider() = 0;
  SPC_NODISCARD virtual iDynamicCollider* CreateDynamicCollider() = 0;
  SPC_NODISCARD virtual iTriggerCollider* CreateTriggerCollider() = 0;

};


}