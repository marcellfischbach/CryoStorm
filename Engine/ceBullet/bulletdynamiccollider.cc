
#include <ceBullet/bulletdynamiccollider.hh>
#include <ceBullet/bulletcollisionshape.hh>
#include <ceBullet/bulletworld.hh>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <ceBullet/bulletutils.hh>


namespace ce::bullet
{

BulletDynamicCollider::BulletDynamicCollider()
  : iDynamicCollider()
  , m_rigidBody(nullptr)
  , m_compoundShape(nullptr)
  , m_world(nullptr)
  , m_userData(nullptr)
{
  m_motionState.priv = this;
  CE_CLASS_GEN_CONSTR;
}

BulletDynamicCollider::~BulletDynamicCollider()
{
  delete m_rigidBody;
}


void BulletDynamicCollider::Attach(iCollisionShape* shape)
{
  if (std::ranges::find(m_shapes, shape) != m_shapes.end())
  {
    return;
  }


  auto btShape = static_cast<BulletCollisionShape*>(shape);
  m_shapes.insert(btShape);

}


void BulletDynamicCollider::Detach(iCollisionShape* shape)
{
  auto it = std::ranges::find(m_shapes, shape);
  if (it == m_shapes.end())
  {
    return;
  }
  m_shapes.erase(it);
}

void BulletDynamicCollider::SetTransform(const Matrix4f& transform)
{
  m_transform = transform;
  if (m_rigidBody)
  {
    btTransform tr;
    Matrix4fTobtTransform(transform, tr);
    m_rigidBody->setWorldTransform(tr);
  }
}


const Matrix4f &BulletDynamicCollider::GetTransform() const
{
  return m_transform;
}


btRigidBody* BulletDynamicCollider::GetRigidBody()
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

void BulletDynamicCollider::SetUserData(SpatialState* userData)
{
  CE_SET(m_userData, userData);
}

SpatialState* BulletDynamicCollider::GetUserData() const
{
  return  m_userData;
}

void BulletDynamicCollider::SetWorld(BulletWorld* world)
{
  CE_SET(m_world, world);
}

void BulletDynamicCollider::MotionState::getWorldTransform(btTransform& worldTrans) const
{
  Matrix4fTobtTransform(priv->m_transform, worldTrans);
}

void BulletDynamicCollider::MotionState::setWorldTransform(const btTransform& worldTrans)
{
  btTransformToMatrix4f(worldTrans, priv->m_transform);

  if (priv->m_world)
  {
    priv->m_world->RegisterUpdate(priv, priv->m_transform);
  }
}

}

