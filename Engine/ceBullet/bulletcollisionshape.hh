
#pragma once

#include <ceBullet/bulletexport.hh>
#include <ceCore/physics/icollisionshape.hh>
#include <btBulletCollisionCommon.h>

namespace ce::bullet
{

CE_CLASS()

class CE_BULLET_API BulletCollisionShape : public CE_SUPER(iCollisionShape)
{
  CE_CLASS_GEN_OBJECT;
public:
  BulletCollisionShape(btCollisionShape * shape);
  ~BulletCollisionShape() override;

  btCollisionShape* GetShape();
  const btCollisionShape* GetShape() const;

private:
  btCollisionShape* m_shape;

};


}
