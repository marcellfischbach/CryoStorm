#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/math/vector3f.hh>

namespace ce
{


CE_CLASS(Virtual)
struct CE_CORE_API iCollisionShape: public CE_SUPER(iObject)
{
  CE_CLASS_GEN;
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