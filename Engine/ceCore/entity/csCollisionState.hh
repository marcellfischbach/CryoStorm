
#pragma once


#include <ceCore/coreexport.hh>
#include <ceCore/entity/csSpatialState.hh>


namespace cryo
{

struct iCollisionShape;
struct iPhysicsSystem;
struct iStaticCollider;

CS_CLASS(Virtual)
class CS_CORE_API csCollisionState : public CS_SUPER(csEntityState)
{
  friend class csRigidBodyState;
  CS_CLASS_GEN;
public:
  csCollisionState(const std::string &name = std::string(""));
  ~csCollisionState() override;

  iCollisionShape *GetShape ();

protected:
  virtual iCollisionShape* CreateShape(iPhysicsSystem *physSystem) const = 0;

private:
  iCollisionShape* m_shape;

};

CS_CLASS()
class CS_CORE_API csSphereColliderState : public CS_SUPER(csCollisionState)
{
  CS_CLASS_GEN;
public:
  csSphereColliderState();
  ~csSphereColliderState() override;

  void SetRadius(float radius);
  float GetRadius() const;

protected:

  iCollisionShape* CreateShape(iPhysicsSystem * physSystem) const override;

private:
  float m_radius;
};

CS_CLASS()
class CS_CORE_API csBoxColliderState : public CS_SUPER(csCollisionState)
{
  CS_CLASS_GEN;
public:
  csBoxColliderState();
  ~csBoxColliderState() override;

  void SetHalfExtends(const Vector3f &halfExtends);
  const Vector3f &GetHalfExtends() const;

protected:

  iCollisionShape* CreateShape(iPhysicsSystem * physSystem) const override;

private:
  Vector3f m_halfExtends;
};

}
