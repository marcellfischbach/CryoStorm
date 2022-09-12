#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/entity/spatialstate.hh>

namespace ce
{

struct iCollisionShape;
struct iPhysicsSystem;
struct iStaticCollider;


CE_CLASS()
class CE_CORE_API StaticColliderState : public CE_SUPER(SpatialState)
{
  CE_CLASS_GEN;

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
