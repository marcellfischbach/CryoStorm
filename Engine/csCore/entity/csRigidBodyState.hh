
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/entity/csSpatialState.hh>

namespace cs
{
struct iCollisionShape;
struct iDynamicCollider;
struct iPhysicsSystem;
struct iPhysicsWorld;

CS_CLASS()
class CS_CORE_API csRigidBodyState : public CS_SUPER(csSpatialState)
{
  CS_CLASS_GEN;

public:
  csRigidBodyState(const std::string &name = std::string(""));
  ~csRigidBodyState() override;

protected:
  void OnAttachedToWorld(csWorld * world) override;
  void OnDetachedFromWorld(csWorld * world) override;

  void TransformationUpdatedPreChildren() override;


private:
  void AcquireEmptyDynamicCollider(iPhysicsSystem *physSystem);
  void DetachCurrentShapes();

  iDynamicCollider *m_dynamicCollider;
  std::vector<iCollisionShape*> m_shapes;
};

}