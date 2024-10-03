
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/entity/spatialstate.hh>

namespace cryo
{
struct iCollisionShape;
struct iDynamicCollider;
struct iPhysicsSystem;
struct iPhysicsWorld;

CS_CLASS()
class CS_CORE_API RigidBodyState : public CS_SUPER(SpatialState)
{
  CS_CLASS_GEN;

public:
  RigidBodyState(const std::string &name = std::string(""));
  ~RigidBodyState() override;

protected:
  void OnAttachedToWorld(World * world) override;
  void OnDetachedFromWorld(World * world) override;

  void TransformationUpdatedPreChildren() override;


private:
  void AcquireEmptyDynamicCollider(iPhysicsSystem *physSystem);
  void DetachCurrentShapes();

  iDynamicCollider *m_dynamicCollider;
  std::vector<iCollisionShape*> m_shapes;
};

}