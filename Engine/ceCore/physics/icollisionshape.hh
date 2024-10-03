#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/math/vector3f.hh>

namespace ce
{


CS_CLASS(Virtual)
struct CS_CORE_API iCollisionShape: public CS_SUPER(iObject)
{
  CS_CLASS_GEN;
  ~iCollisionShape() override = default;


};


struct BoxShapeDesc
{
  Vector3f HalfExtents;
};

struct SphereShapeDesc
{
  float Radius;
};

struct CylinderShapeDesc
{
  enum class eAxis { X, Y, Z };

  eAxis Axis;
  float Radius;
  float HalfHeight;
};

struct CapsuleShapeDesc
{
  enum class eAxis { X, Y, Z };

  eAxis Axis;
  float Radius;
  float HalfHeight;
};

}