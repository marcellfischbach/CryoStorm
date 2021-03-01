



#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/graphics/ilight.hh>
#include <spcCore/math/vector3f.hh>


namespace spc
{


SPC_CLASS()
struct SPC_CORE_API iPointLight : public SPC_SUPER(iLight)
{
  SPC_CLASS_GEN;
  virtual ~iPointLight() { }

  virtual void SetPosition(const Vector3f & direction) = 0;
  virtual SPC_NODISCARD const Vector3f& GetPosition() const = 0;

  virtual void SetRange(float range) = 0;
  virtual SPC_NODISCARD float GetRange() const = 0;


};

}