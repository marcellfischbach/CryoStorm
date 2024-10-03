



#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/graphics/ilight.hh>
#include <ceCore/math/vector3f.hh>


namespace cryo
{


CS_CLASS()
struct CS_CORE_API iPointLight : public CS_SUPER(iLight)
{
  CS_CLASS_GEN;
  virtual ~iPointLight() { }

  virtual void SetPosition(const Vector3f & direction) = 0;
  virtual CS_NODISCARD const Vector3f& GetPosition() const = 0;

  virtual void SetRange(float range) = 0;
  virtual CS_NODISCARD float GetRange() const = 0;


};

}