
#pragma once

#include <spcBullet/bulletexport.hh>
#include <spcCore/physics/icollisionshape.hh>
#include <btBulletCollisionCommon.h>

namespace spc::bullet
{

SPC_CLASS()

class SPC_BULLET_API BulletCollisionShape : public SPC_SUPER(iCollisionShape)
{
  SPC_CLASS_GEN_OBJECT;
public:
  BulletCollisionShape(btCollisionShape * shape);
  ~BulletCollisionShape() override;

  btCollisionShape* GetShape();
  const btCollisionShape* GetShape() const;

private:
  btCollisionShape* m_shape;

};


}
