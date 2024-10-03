
#pragma once


#include <ceCore/coreexport.hh>
#include <ceCore/entity/spatialstate.hh>


namespace cryo
{

struct iCollisionShape;
struct iPhysicsSystem;
struct iStaticCollider;

CS_CLASS(Virtual)
class CS_CORE_API CollisionState : public CS_SUPER(EntityState)
{
  friend class RigidBodyState;
  CS_CLASS_GEN;
public:
  CollisionState(const std::string &name = std::string(""));
  ~CollisionState() override;

  iCollisionShape *GetShape ();

protected:
  virtual iCollisionShape* CreateShape(iPhysicsSystem *physSystem) const = 0;

private:
  iCollisionShape* m_shape;

};

CS_CLASS()
class CS_CORE_API SphereColliderState : public CS_SUPER(CollisionState)
{
  CS_CLASS_GEN;
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

CS_CLASS()
class CS_CORE_API BoxColliderState : public CS_SUPER(CollisionState)
{
  CS_CLASS_GEN;
public:
  BoxColliderState();
  ~BoxColliderState() override;

  void SetHalfExtends(const Vector3f &halfExtends);
  const Vector3f &GetHalfExtends() const;

protected:

  iCollisionShape* CreateShape(iPhysicsSystem * physSystem) const override;

private:
  Vector3f m_halfExtends;
};

}
