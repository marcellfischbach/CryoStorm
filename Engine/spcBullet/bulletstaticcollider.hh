
#pragma once

#include <spcBullet/bulletexport.hh>
#include <spcCore/physics/icollider.hh>
#include <spcCore/math/matrix4f.hh>
#include <set>

class btCollisionObject;
class btCompoundShape;

namespace spc::bullet
{

class BulletCollisionShape;

SPC_CLASS()
class BulletStaticCollider : public SPC_SUPER(iStaticCollider)
{
  SPC_CLASS_GEN_OBJECT;
public:
  BulletStaticCollider();
  ~BulletStaticCollider() override;

  void Attach(iCollisionShape * shape) override;
  void Detach(iCollisionShape * shape) override;

  void SetTransform(const Matrix4f & transform) override;
  const Matrix4f& GetTransform() const override;


  btCollisionObject* GetCollisionObject();

private:
  btCollisionObject* m_collisionObject;

  btCompoundShape* m_compoundShape;
  std::set<BulletCollisionShape*> m_shapes;
  Matrix4f m_transform;
};


}
