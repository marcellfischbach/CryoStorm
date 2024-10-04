
#pragma once

#include <csBullet/csBulletExport.hh>
#include <csCore/physics/iCollider.hh>
#include <set>
#include <btBulletDynamicsCommon.h>

class btRigidBody;
class btCompoundShape;

namespace cryo::bullet
{

class csBulletCollisionShape;
class csBulletWorld;

CS_CLASS()
class csBulletDynamicCollider : public CS_SUPER(iDynamicCollider)
{
  CS_CLASS_GEN_OBJECT;
public:
  csBulletDynamicCollider();
  ~csBulletDynamicCollider() override;

  void Attach(iCollisionShape * shape) override;
  void Detach(iCollisionShape * shape) override;

  void SetTransform(const csMatrix4f & transform) override;
  const csMatrix4f &GetTransform() const override;


  void SetUserData(csSpatialState * spatialState) override;
  csSpatialState* GetUserData() const override;

  btRigidBody* GetRigidBody();

  void SetWorld(csBulletWorld * world);

private:

  btRigidBody* m_rigidBody;

  btCompoundShape* m_compoundShape;
  std::set<csBulletCollisionShape*> m_shapes;

  struct MotionState : public btMotionState
  {
    csBulletDynamicCollider * priv;
    void getWorldTransform(btTransform& worldTrans) const override;

    void setWorldTransform(const btTransform& worldTrans) override;
  } m_motionState;

  csMatrix4f m_transform;


  csBulletWorld * m_world;

  csSpatialState * m_userData;
};


}
