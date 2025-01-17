#include <csBullet/csBulletSystem.hh>
#include <csBullet/csBulletWorld.hh>
#include <csBullet/csBulletCollisionShape.hh>
#include <csBullet/csBulletDynamicCollider.hh>
#include <csBullet/csBulletStaticCollider.hh>
#include <btBulletCollisionCommon.h>


namespace cs::bullet
{

csBulletSystem::csBulletSystem()
{

}

csBulletSystem::~csBulletSystem()
{

}


csOwned<iPhysicsWorld> csBulletSystem::CreateWorld()
{
  return new csBulletWorld();
}


csOwned<iCollisionShape> csBulletSystem::CreateShape(const SphereShapeDesc& desc)
{
  btSphereShape* shape = new btSphereShape(desc.Radius);
  return new csBulletCollisionShape(shape);
}


csOwned<iCollisionShape> csBulletSystem::CreateShape(const BoxShapeDesc& desc)
{
  btBoxShape* shape = new btBoxShape(btVector3(desc.HalfExtents.x, desc.HalfExtents.y, desc.HalfExtents.z));
  return new csBulletCollisionShape(shape);
}

csOwned<iCollisionShape> csBulletSystem::CreateShape(const CylinderShapeDesc& desc)
{
  btCylinderShape* shape;
  switch (desc.Axis)
  {
  case CylinderShapeDesc::eAxis::X:
    shape = new btCylinderShapeX(btVector3(desc.HalfHeight, desc.Radius, desc.Radius));
    break;
  case CylinderShapeDesc::eAxis::Y:
    shape = new btCylinderShape(btVector3(desc.Radius, desc.HalfHeight, desc.Radius));
    break;
  case CylinderShapeDesc::eAxis::Z:
    shape = new btCylinderShapeZ(btVector3(desc.Radius, desc.Radius, desc.HalfHeight));
    break;
  default:
    return nullptr;
  }

  return new csBulletCollisionShape(shape);
}

csOwned<iCollisionShape> csBulletSystem::CreateShape(const CapsuleShapeDesc& desc)
{
  btCapsuleShape* shape;
  switch (desc.Axis)
  {
  case CapsuleShapeDesc::eAxis::X:
    shape = new btCapsuleShapeX(desc.Radius, desc.HalfHeight * 2.0f);
    break;
  case CapsuleShapeDesc::eAxis::Y:
    shape = new btCapsuleShape(desc.Radius, desc.HalfHeight * 2.0f);
    break;
  case CapsuleShapeDesc::eAxis::Z:
    shape = new btCapsuleShapeZ(desc.Radius, desc.HalfHeight * 2.0f);
    break;
  default:
    return nullptr;
  }

  return new csBulletCollisionShape(shape);

}



csOwned<iStaticCollider> csBulletSystem::CreateStaticCollider()
{
  return new csBulletStaticCollider();
}

csOwned<iDynamicCollider> csBulletSystem::CreateDynamicCollider()
{
  return new csBulletDynamicCollider();
}

csOwned<iTriggerCollider> csBulletSystem::CreateTriggerCollider()
{
  return nullptr;
}


}