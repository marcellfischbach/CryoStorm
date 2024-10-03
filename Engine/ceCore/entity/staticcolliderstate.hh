#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/entity/spatialstate.hh>

namespace ce
{

struct iCollisionShape;
struct iPhysicsSystem;
struct iStaticCollider;


CS_CLASS()
class CS_CORE_API StaticColliderState : public CS_SUPER(SpatialState)
{
  CS_CLASS_GEN;

public:
  StaticColliderState(const std::string &name = std::string (""));
  ~StaticColliderState() override;

protected:

  void OnAttachedToWorld(World * world) override;
  void OnDetachedFromWorld(World * world) override;

  void TransformationUpdatedPreChildren() override;

private:
  void AcquireEmptyStaticCollider(iPhysicsSystem* physicsSystem);
  void DetachCurrentShapes();

  iStaticCollider* m_collider;
  std::vector<iCollisionShape*> m_shapes;
};

} // ce
