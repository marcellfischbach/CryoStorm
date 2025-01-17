
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/csRef.hh>
namespace cs
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


  CS_NODISCARD virtual csOwned<iPhysicsWorld> CreateWorld() = 0;

  CS_NODISCARD virtual csOwned<iCollisionShape> CreateShape(const SphereShapeDesc & desc) = 0;
  CS_NODISCARD virtual csOwned<iCollisionShape> CreateShape(const BoxShapeDesc & desc) = 0;
  CS_NODISCARD virtual csOwned<iCollisionShape> CreateShape(const CylinderShapeDesc & desc) = 0;
  CS_NODISCARD virtual csOwned<iCollisionShape> CreateShape(const CapsuleShapeDesc & desc) = 0;


  CS_NODISCARD virtual csOwned<iStaticCollider> CreateStaticCollider() = 0;
  CS_NODISCARD virtual csOwned<iDynamicCollider> CreateDynamicCollider() = 0;
  CS_NODISCARD virtual csOwned<iTriggerCollider> CreateTriggerCollider() = 0;

};


}