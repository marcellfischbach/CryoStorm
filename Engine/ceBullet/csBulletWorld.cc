
#include <ceBullet/csBulletWorld.hh>
#include <ceBullet/csBulletDynamicCollider.hh>
#include <ceBullet/csBulletStaticCollider.hh>
#include <btBulletDynamicsCommon.h>

namespace cryo::bullet
{


csBulletWorld::csBulletWorld()
  : m_writeId(0)
  , m_tmpId(1)
  , m_exportId(2)
{
  CS_CLASS_GEN_CONSTR;

  


  m_configuration = new btDefaultCollisionConfiguration();
  m_dispatcher = new btCollisionDispatcher(m_configuration);
  m_broadphase = new btDbvtBroadphase();
  m_constraintSolver = new btSequentialImpulseConstraintSolver();
  m_world = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_constraintSolver, m_configuration);
  m_world->setGravity(btVector3(0.0f, -9.81f, 0.0f));
}

csBulletWorld::~csBulletWorld()
{

}


void csBulletWorld::Step(float tpf)
{
  m_resultFrame[m_writeId].clear();
  m_world->stepSimulation(tpf);

  // swap the temp and the write index
  unsigned tmp = m_tmpId;
  m_tmpId = m_writeId;
  m_writeId = tmp;
}

const std::vector<iPhysicsWorld::DynamicResult>& csBulletWorld::SwapResult()
{
  // swap the temp and the export index
  unsigned tmp = m_tmpId;
  m_tmpId = m_exportId;
  m_exportId = tmp;

  return m_resultFrame[m_exportId];
}


void csBulletWorld::AddCollider(iStaticCollider* collider)
{

  auto staticCollider = static_cast<csBulletStaticCollider*>(collider);
  auto object = staticCollider->GetCollisionObject();
  if (object)
  {
    m_world->addCollisionObject(object);
  }
}

void csBulletWorld::AddCollider(iDynamicCollider* collider)
{
  auto dynamicCollider = static_cast<csBulletDynamicCollider*>(collider);
  dynamicCollider->SetWorld(this);
  auto body = dynamicCollider->GetRigidBody();
  if (body)
  {
    m_world->addRigidBody(body);
  }

}


void csBulletWorld::RemoveCollider(iStaticCollider* collider)
{
  auto staticCollider = static_cast<csBulletStaticCollider*>(collider);
  auto object = staticCollider->GetCollisionObject();
  if (object)
  {
    m_world->removeCollisionObject(object);
  }

}

void csBulletWorld::RemoveCollider(iDynamicCollider* collider)
{
  auto dynamicCollider = static_cast<csBulletDynamicCollider*>(collider);
  dynamicCollider->SetWorld(nullptr);
  auto body = dynamicCollider->GetRigidBody();
  if (body)
  {
    m_world->removeRigidBody(body);
  }

}


void csBulletWorld::RegisterUpdate(csBulletDynamicCollider* collider, const csMatrix4f& matrix)
{
  DynamicResult res{ collider, matrix };
  m_resultFrame[m_writeId].emplace_back(res);
}


}