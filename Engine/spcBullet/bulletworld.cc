
#include <spcBullet/bulletworld.hh>
#include <spcBullet/bulletdynamiccollider.hh>
#include <spcBullet/bulletstaticcollider.hh>
#include <btBulletDynamicsCommon.h>

namespace spc::bullet
{


BulletWorld::BulletWorld()
  : m_writeId(0)
  , m_tmpId(1)
  , m_exportId(2)
{
  SPC_CLASS_GEN_CONSTR;

  


  m_configuration = new btDefaultCollisionConfiguration();
  m_dispatcher = new btCollisionDispatcher(m_configuration);
  m_broadphase = new btDbvtBroadphase();
  m_constraintSolver = new btSequentialImpulseConstraintSolver();
  m_world = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_constraintSolver, m_configuration);
  m_world->setGravity(btVector3(0.0f, -9.81f, 0.0f));
}

BulletWorld::~BulletWorld()
{

}


void BulletWorld::Step(float tpf)
{
  m_resultFrame[m_writeId].clear();
  m_world->stepSimulation(tpf);

  // swap the temp and the write index
  unsigned tmp = m_tmpId;
  m_tmpId = m_writeId;
  m_writeId = tmp;
}

const std::vector<iPhysicsWorld::DynamicResult>& BulletWorld::SwapResult()
{
  // swap the temp and the export index
  unsigned tmp = m_tmpId;
  m_tmpId = m_exportId;
  m_exportId = tmp;

  return m_resultFrame[m_exportId];
}


void BulletWorld::AddCollider(iStaticCollider* collider)
{

  auto staticCollider = static_cast<BulletStaticCollider*>(collider);
  auto object = staticCollider->GetCollisionObject();
  if (object)
  {
    m_world->addCollisionObject(object);
  }
}

void BulletWorld::AddCollider(iDynamicCollider* collider)
{
  auto dynamicCollider = static_cast<BulletDynamicCollider*>(collider);
  dynamicCollider->SetWorld(this);
  auto body = dynamicCollider->GetRigidBody();
  if (body)
  {
    m_world->addRigidBody(body);
  }

}


void BulletWorld::RemoveCollider(iStaticCollider* collider)
{
  auto staticCollider = static_cast<BulletStaticCollider*>(collider);
  auto object = staticCollider->GetCollisionObject();
  if (object)
  {
    m_world->removeCollisionObject(object);
  }

}

void BulletWorld::RemoveCollider(iDynamicCollider* collider)
{
  auto dynamicCollider = static_cast<BulletDynamicCollider*>(collider);
  dynamicCollider->SetWorld(nullptr);
  auto body = dynamicCollider->GetRigidBody();
  if (body)
  {
    m_world->removeRigidBody(body);
  }

}


void BulletWorld::RegisterUpdate(BulletDynamicCollider* collider, const Matrix4f& matrix)
{
  DynamicResult res{ collider, matrix };
  m_resultFrame[m_writeId].emplace_back(res);
}


}