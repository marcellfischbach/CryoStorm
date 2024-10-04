
#pragma once

#include <ceBullet/csBulletExport.hh>
#include <ceCore/physics/iCollider.hh>
#include <ceCore/math/csMatrix4f.hh>
#include <set>

class btCollisionObject;
class btCompoundShape;

namespace cryo::bullet
{

class csBulletCollisionShape;

CS_CLASS()
class csBulletStaticCollider : public CS_SUPER(iStaticCollider)
{
  CS_CLASS_GEN_OBJECT;
public:
  csBulletStaticCollider();
  ~csBulletStaticCollider() override;

  void Attach(iCollisionShape * shape) override;
  void Detach(iCollisionShape * shape) override;

  void SetTransform(const csMatrix4f & transform) override;
  const csMatrix4f& GetTransform() const override;


  btCollisionObject* GetCollisionObject();

private:
  btCollisionObject* m_collisionObject;

  btCompoundShape* m_compoundShape;
  std::set<csBulletCollisionShape*> m_shapes;
  csMatrix4f                        m_transform;
};


}
