#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/math/csVector3f.hh>

namespace cryo
{


CS_CLASS(Virtual)
struct CS_CORE_API iCollisionShape: public CS_SUPER(iObject)
{
  CS_CLASS_GEN;
  ~iCollisionShape() override = default;


};


struct BoxShapeDesc
{
  csVector3f HalfExtents;
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