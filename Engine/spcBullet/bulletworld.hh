
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


class BulletDynamicCollider;
SPC_CLASS()
class SPC_BULLET_API BulletWorld : public SPC_SUPER(iPhysicsWorld)
{
  SPC_CLASS_GEN_OBJECT;
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
