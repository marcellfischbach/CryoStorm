
#include <ceBullet/csBulletCollisionShape.hh>
#include <btBulletCollisionCommon.h>


namespace cryo::bullet
{



csBulletCollisionShape::csBulletCollisionShape(btCollisionShape* shape)
  : iCollisionShape()
  , m_shape(shape)
{

}


csBulletCollisionShape::~csBulletCollisionShape()
{
  delete m_shape;
}

btCollisionShape* csBulletCollisionShape::GetShape()
{
  return m_shape;
}

const btCollisionShape* csBulletCollisionShape::GetShape() const
{
  return m_shape;
}


}