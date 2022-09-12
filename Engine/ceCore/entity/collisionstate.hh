
#pragma once


#include <ceCore/coreexport.hh>
#include <ceCore/entity/spatialstate.hh>


namespace ce
{

struct iCollisionShape;
struct iPhysicsSystem;
struct iStaticCollider;

CE_CLASS(Virtual)
class CE_CORE_API CollisionState : public CE_SUPER(EntityState)
{
  friend class RigidBodyState;
  CE_CLASS_GEN;
public:
  CollisionState(const std::string &name = std::string(""));
  ~CollisionState() override;

  iCollisionShape *GetShape ();

protected:
  virtual iCollisionShape* CreateShape(iPhysicsSystem *physSystem) const = 0;

private:
  iCollisionShape* m_shape;

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

CE_CLASS()
class CE_CORE_API BoxColliderState : public CE_SUPER(CollisionState)
{
  CE_CLASS_GEN;
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
