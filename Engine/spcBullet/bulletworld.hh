
#pragma once


#include <spcBullet/bulletexport.hh>
#include <spcCore/physics/iphysicsworld.hh>



class btBroadphaseInterface;
class btCollisionConfiguration;
class btCollisionDispatcher;
class btDiscreteDynamicsWorld;
class btSequentialImpulseConstraintSolver;

namespace spc::bullet
{


SPC_CLASS()
class SPC_BULLET_API BulletWorld : public SPC_SUPER(iPhysicsWorld)
{
  SPC_CLASS_GEN_OBJECT;
public:
  BulletWorld();
  ~BulletWorld() override;

  void Step(float tpf) override;

private:
  btDiscreteDynamicsWorld *m_world;
  btCollisionDispatcher *m_dispatcher;
  btBroadphaseInterface *m_broadphase;
  btSequentialImpulseConstraintSolver *m_constraintSolver;
  btCollisionConfiguration *m_configuration;
};

}
