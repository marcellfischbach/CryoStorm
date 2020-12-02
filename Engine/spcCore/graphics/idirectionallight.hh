

#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/graphics/ilight.hh>
#include <spcCore/math/vector3f.hh>


namespace spc
{


SPC_CLASS()
struct SPC_CORE_API iDirectionalLight : public SPC_SUPER(iLight)
{
  SPC_CLASS_GEN;
  virtual ~iDirectionalLight() { }

  virtual void SetDirection(const Vector3f & direction) = 0;
  virtual const Vector3f& GetDirection() const = 0;


};

}