
#pragma once

#include <ceBullet/bulletexport.hh>
#include <ceCore/physics/icollider.hh>
#include <ceCore/math/matrix4f.hh>
#include <set>

class btCollisionObject;
class btCompoundShape;

namespace ce::bullet
{

class BulletCollisionShape;

CS_CLASS()
class BulletStaticCollider : public CS_SUPER(iStaticCollider)
{
  CS_CLASS_GEN_OBJECT;
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
