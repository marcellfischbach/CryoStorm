
#include <csBullet/csBulletDynamicCollider.hh>
#include <csBullet/csBulletCollisionShape.hh>
#include <csBullet/csBulletWorld.hh>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <csBullet/csBulletUtils.hh>
#include <algorithm>

namespace cs::bullet
{

csBulletDynamicCollider::csBulletDynamicCollider()
  : iDynamicCollider()
  , m_rigidBody(nullptr)
  , m_compoundShape(nullptr)
  , m_world(nullptr)
  , m_userData(nullptr)
{
  m_motionState.priv = this;

}

csBulletDynamicCollider::~csBulletDynamicCollider()
{
  delete m_rigidBody;
}


void csBulletDynamicCollider::Attach(iCollisionShape* shape)
{
  if (std::find(m_shapes.begin(), m_shapes.end(), shape) != m_shapes.end())
  {
    return;
  }


  auto btShape = static_cast<csBulletCollisionShape*>(shape);
  m_shapes.insert(btShape);

}


void csBulletDynamicCollider::Detach(iCollisionShape* shape)
{
  auto it = std::find(m_shapes.begin(), m_shapes.end(), shape);
  if (it == m_shapes.end())
  {
    return;
  }
  m_shapes.erase(it);
}

void csBulletDynamicCollider::SetTransform(const csMatrix4f& transform)
{
  m_transform = transform;
  if (m_rigidBody)
  {
    btTransform tr;
    Matrix4fTobtTransform(transform, tr);
    m_rigidBody->setWorldTransform(tr);
  }
}


const csMatrix4f &csBulletDynamicCollider::GetTransform() const
{
  return m_transform;
}


btRigidBody* csBulletDynamicCollider::GetRigidBody()
{
  if (!m_rigidBody)
  {

    btCollisionShape* finalShape = nullptr;
    if (m_shapes.size() > 1)
    {
      m_compoundShape = new btCompoundShape();
      for (auto shape : m_shapes)
      {
        btTransform tr;
        tr.setIdentity();
        m_compoundShape->addChildShape(tr, shape->GetShape());
      }

      finalShape = m_compoundShape;
    }
    else if (m_shapes.size() == 1)
    {
      auto it = m_shapes.begin();
      finalShape = (*it)->GetShape();
    }

    if (finalShape)
    {
      float mass = 1.0f;
      btVector3 localInertia;
      finalShape->calculateLocalInertia(mass, localInertia);
      m_rigidBody = new btRigidBody(1.0, &m_motionState, finalShape, localInertia);
    }


    m_rigidBody->setRestitution(0.0f);
    m_rigidBody->setRollingFriction(1.0f);
    m_rigidBody->setFriction(0.1f);
  }
  return m_rigidBody;
}

void csBulletDynamicCollider::SetUserData(csSpatialState* userData)
{
  CS_SET(m_userData, userData);
}

csSpatialState* csBulletDynamicCollider::GetUserData() const
{
  return  m_userData;
}

void csBulletDynamicCollider::SetWorld(csBulletWorld* world)
{
  CS_SET(m_world, world);
}

void csBulletDynamicCollider::MotionState::getWorldTransform(btTransform& worldTrans) const
{
  Matrix4fTobtTransform(priv->m_transform, worldTrans);
}

void csBulletDynamicCollider::MotionState::setWorldTransform(const btTransform& worldTrans)
{
  btTransformToMatrix4f(worldTrans, priv->m_transform);

  if (priv->m_world)
  {
    priv->m_world->RegisterUpdate(priv, priv->m_transform);
  }
}

}

