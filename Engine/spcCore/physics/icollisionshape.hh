#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>
#include <spcCore/math/vector3f.hh>

namespace spc
{


SPC_CLASS(Virtual)
struct SPC_CORE_API iCollisionShape: public SPC_SUPER(iObject)
{
  SPC_CLASS_GEN;
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