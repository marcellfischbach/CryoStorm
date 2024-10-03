
#pragma once

#include <ceBullet/bulletexport.hh>
#include <ceCore/physics/icollisionshape.hh>

class btCollisionShape;

namespace ce::bullet
{

CS_CLASS()

class CS_BULLET_API BulletCollisionShape : public CS_SUPER(iCollisionShape)
{
  CS_CLASS_GEN_OBJECT;
public:
  BulletCollisionShape(btCollisionShape * shape);
  ~BulletCollisionShape() override;

  btCollisionShape* GetShape();
  const btCollisionShape* GetShape() const;

private:
  btCollisionShape* m_shape;

};


}
