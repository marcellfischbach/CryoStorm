
#pragma once

#include <csBullet/csBulletExport.hh>
#include <csCore/physics/iCollisionShape.hh>

class btCollisionShape;

namespace cryo::bullet
{

CS_CLASS()

class CS_BULLET_API csBulletCollisionShape : public CS_SUPER(iCollisionShape)
{
  CS_CLASS_GEN_OBJECT;
public:
  csBulletCollisionShape(btCollisionShape * shape);
  ~csBulletCollisionShape() override;

  btCollisionShape* GetShape();
  const btCollisionShape* GetShape() const;

private:
  btCollisionShape* m_shape;

};


}
