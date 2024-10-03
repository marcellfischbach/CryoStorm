
#pragma once


#include <ceBullet/bulletexport.hh>
#include <ceCore/physics/iphysicsworld.hh>



class btBroadphaseInterface;
class btCollisionConfiguration;
class btCollisionDispatcher;
class btDiscreteDynamicsWorld;
class btSequentialImpulseConstraintSolver;

namespace cryo::bullet
{


class BulletDynamicCollider;
CS_CLASS()
class CS_BULLET_API BulletWorld : public CS_SUPER(iPhysicsWorld)
{
  CS_CLASS_GEN_OBJECT;
public:
  BulletWorld();
  ~BulletWorld() override;

  void Step(float tpf) override;
  const std::vector<DynamicResult>& SwapResult() override;


  void AddCollider(iStaticCollider * collider) override;
  void AddCollider(iDynamicCollider * collider) override;


  void RemoveCollider(iStaticCollider * collider) override;
  void RemoveCollider(iDynamicCollider * collider) override;

  void RegisterUpdate(BulletDynamicCollider * collider, const Matrix4f & transform);

private:
  btDiscreteDynamicsWorld *m_world;
  btCollisionDispatcher *m_dispatcher;
  btBroadphaseInterface *m_broadphase;
  btSequentialImpulseConstraintSolver *m_constraintSolver;
  btCollisionConfiguration *m_configuration;

  unsigned m_exportId;
  unsigned m_writeId;
  unsigned m_tmpId;
  std::vector<DynamicResult> m_resultFrame[3];
};

}
