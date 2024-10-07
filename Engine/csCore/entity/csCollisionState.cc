#include <csCore/entity/csCollisionState.hh>
#include <csCore/entity/csRigidBodyState.hh>
#include <csCore/entity/csWorld.hh>
#include <csCore/physics/iPhysicsSystem.hh>
#include <csCore/physics/iCollider.hh>
#include <csCore/physics/iCollisionShape.hh>
#include <csCore/physics/iPhysicsWorld.hh>
#include <csCore/csObjectRegistry.hh>

namespace cs
{


csCollisionState::csCollisionState(const std::string &name)
  : csEntityState(name)
  , m_shape(nullptr)
{
}

csCollisionState::~csCollisionState()
{

}


iCollisionShape* csCollisionState::GetShape()
{
  if (!m_shape)
  {
    auto physSystem = csObjectRegistry::Get<iPhysicsSystem>();
    if (physSystem)
    {
      m_shape = CreateShape(physSystem);
    }
  }
  return m_shape;
}





csSphereColliderState::csSphereColliderState()
  : csCollisionState()
  , m_radius(1.0f)
{

}

csSphereColliderState::~csSphereColliderState()
{

}





iCollisionShape* csSphereColliderState::CreateShape(iPhysicsSystem* physSystem) const
{
  SphereShapeDesc desc = {};
  desc.Radius = m_radius;

  return physSystem->CreateShape(desc);
}


void csSphereColliderState::SetRadius(float radius)
{
  m_radius = radius;
}

float csSphereColliderState::GetRadius() const
{
  return m_radius;
}









csBoxColliderState::csBoxColliderState()
  : csCollisionState()
  , m_halfExtends(1.0f, 1.0f, 1.0f)
{

}

csBoxColliderState::~csBoxColliderState()
{

}





iCollisionShape* csBoxColliderState::CreateShape(iPhysicsSystem* physSystem) const
{
  BoxShapeDesc desc = {};
  desc.HalfExtents = m_halfExtends;

  return physSystem->CreateShape(desc);
}


void csBoxColliderState::SetHalfExtends(const csVector3f &halfExtends)
{
  m_halfExtends = halfExtends;
}

const csVector3f &csBoxColliderState::GetHalfExtends() const
{
  return m_halfExtends;
}


}
