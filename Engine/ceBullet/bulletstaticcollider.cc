
#include <ceBullet/bulletstaticcollider.hh>
#include <ceBullet/bulletcollisionshape.hh>
#include <ceBullet/bulletutils.hh>
#include <btBulletCollisionCommon.h>
#include <algorithm>

namespace ce::bullet
{

BulletStaticCollider::BulletStaticCollider()
  : iStaticCollider()
  , m_collisionObject(new btCollisionObject())
  , m_compoundShape(nullptr)
{
  m_collisionObject->setFriction(1.0f);
  m_collisionObject->setRollingFriction(0.1f);

  CE_CLASS_GEN_CONSTR;
}

BulletStaticCollider::~BulletStaticCollider()
{
  delete m_collisionObject;
}


void BulletStaticCollider::Attach(iCollisionShape* shape)
{
  if (std::ranges::find(m_shapes, shape) != m_shapes.end())
  {
    return;
  }


  auto btShape = static_cast<BulletCollisionShape*>(shape);
  m_shapes.insert(btShape);

  if (m_shapes.size() == 1)
  {
    m_collisionObject->setCollisionShape(btShape->GetShape());
    return;
  }

  if (!m_compoundShape)
  {
    m_compoundShape = new btCompoundShape();
    m_collisionObject->setCollisionShape(m_compoundShape);
  }


  btTransform tr;
  tr.setIdentity();
  m_compoundShape->addChildShape(tr, btShape->GetShape());
  m_shapes.insert(btShape);

}

void BulletStaticCollider::Detach(iCollisionShape* shape)
{
  auto it = std::ranges::find(m_shapes, shape);
  if (it == m_shapes.end())
  {
    return;
  }
  m_shapes.erase(it);

  auto btShape = static_cast<BulletCollisionShape*>(shape);
  if (m_compoundShape)
  {
    m_compoundShape->removeChildShape(btShape->GetShape());
    if (m_compoundShape->getNumChildShapes() == 0)
    {
      m_collisionObject->setCollisionShape(nullptr);
    }
    delete m_compoundShape;
    m_compoundShape = nullptr;
  }
  else if (m_collisionObject->getCollisionShape() == btShape->GetShape())
  {
    m_collisionObject->setCollisionShape(nullptr);
  }

}



void BulletStaticCollider::SetTransform(const Matrix4f& transform)
{
  m_transform = transform;
  if (m_collisionObject)
  {
    btTransform tr;
    Matrix4fTobtTransform(transform, tr);
    m_collisionObject->setWorldTransform(tr);
  }
}


const Matrix4f& BulletStaticCollider::GetTransform() const
{
  return m_transform;
}


btCollisionObject* BulletStaticCollider::GetCollisionObject()
{
  return m_collisionObject;
}

}

