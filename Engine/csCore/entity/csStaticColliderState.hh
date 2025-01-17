#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/entity/csSpatialState.hh>

namespace cs
{

struct iCollisionShape;
struct iPhysicsSystem;
struct iStaticCollider;


CS_CLASS()
class CS_CORE_API csStaticColliderState : public CS_SUPER(csSpatialState)
{
  CS_CLASS_GEN;

public:
  csStaticColliderState(const std::string &name = std::string (""));
  ~csStaticColliderState() override;

protected:

  void OnAttachedToWorld(csWorld * world) override;
  void OnDetachedFromWorld(csWorld * world) override;

  void TransformationUpdatedPreChildren() override;

private:
  void AcquireEmptyStaticCollider(iPhysicsSystem* physicsSystem);
  void DetachCurrentShapes();

  csRef<iStaticCollider> m_collider;
  std::vector<csRef<iCollisionShape>> m_shapes;
};

} // ce
