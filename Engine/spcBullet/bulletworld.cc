
#include <spcBullet/bulletworld.hh>
#include <btBulletDynamicsCommon.h>

namespace spc::bullet
{


BulletWorld::BulletWorld()
{
  m_configuration = new btDefaultCollisionConfiguration();
  m_dispatcher = new btCollisionDispatcher(m_configuration);
  m_broadphase = new btDbvtBroadphase();
  m_constraintSolver = new btSequentialImpulseConstraintSolver();
  m_world = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_constraintSolver, m_configuration);
}

BulletWorld::~BulletWorld()
{

}


void BulletWorld::Step(float tpf)
{
  m_world->stepSimulation(tpf);
}

}