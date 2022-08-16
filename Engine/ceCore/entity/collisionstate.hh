
#pragma once


#include <ceCore/coreexport.hh>
#include <ceCore/entity/spatialstate.hh>


namespace ce
{

struct iCollider;
struct iCollisionShape;
struct iPhysicsSystem;

CE_CLASS()
class CE_CORE_API CollisionState : public CE_SUPER(SpatialState)
{
  CE_CLASS_GEN_OBJECT;
public:
  CollisionState();
  ~CollisionState() override;


protected:
  virtual iCollisionShape* CreateShape(iPhysicsSystem *physSystem) const = 0;

  void OnAttachedToWorld(World * world) override;
  void OnDetachedFromWorld(World * world) override;

private:
  iCollider* m_collider;


};

CE_CLASS()
class CE_CORE_API SphereColliderState : public CE_SUPER(CollisionState)
{
  CE_CLASS_GEN;
public:
  SphereColliderState();
  ~SphereColliderState() override;

  void SetRadius(float radius);
  float GetRadius() const;

protected:

  iCollisionShape* CreateShape(iPhysicsSystem * physSystem) const override;

private:
  float m_radius;
};


}
