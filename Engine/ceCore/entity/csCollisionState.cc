#include <ceCore/entity/csCollisionState.hh>
#include <ceCore/entity/csRigidBodyState.hh>
#include <ceCore/entity/csWorld.hh>
#include <ceCore/physics/iPhysicsSystem.hh>
#include <ceCore/physics/iCollider.hh>
#include <ceCore/physics/iCollisionShape.hh>
#include <ceCore/physics/iPhysicsWorld.hh>
#include <ceCore/csObjectRegistry.hh>

namespace cryo
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
