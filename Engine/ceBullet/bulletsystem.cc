#include <ceBullet/bulletsystem.hh>
#include <ceBullet/bulletworld.hh>
#include <ceBullet/bulletcollisionshape.hh>
#include <ceBullet/bulletdynamiccollider.hh>
#include <ceBullet/bulletstaticcollider.hh>
#include <btBulletCollisionCommon.h>


namespace ce::bullet
{

BulletSystem::BulletSystem()
{
  CE_CLASS_GEN_CONSTR;
}

BulletSystem::~BulletSystem()
{

}


iPhysicsWorld *BulletSystem::CreateWorld() 
{
  return new BulletWorld();
}


iCollisionShape* BulletSystem::CreateShape(const SphereShapeDesc& desc)
{
  btSphereShape* shape = new btSphereShape(desc.Radius);
  return new BulletCollisionShape(shape);
}


iCollisionShape* BulletSystem::CreateShape(const BoxShapeDesc& desc)
{
  btBoxShape* shape = new btBoxShape(btVector3(desc.HalfExtents.x, desc.HalfExtents.y, desc.HalfExtents.z));
  return new BulletCollisionShape(shape);
}

iCollisionShape* BulletSystem::CreateShape(const CylinderShapeDesc& desc)
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

  return new BulletCollisionShape(shape);
}

iCollisionShape* BulletSystem::CreateShape(const CapsuleShapeDesc& desc)
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

  return new BulletCollisionShape(shape);

}



iStaticCollider* BulletSystem::CreateStaticCollider()
{
  return new BulletStaticCollider();
}

iDynamicCollider* BulletSystem::CreateDynamicCollider()
{
  return new BulletDynamicCollider();
}

iTriggerCollider* BulletSystem::CreateTriggerCollider()
{
  return nullptr;
}


}