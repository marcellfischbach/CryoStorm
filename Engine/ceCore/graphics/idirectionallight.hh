

#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/graphics/ilight.hh>
#include <ceCore/math/vector3f.hh>


namespace ce
{


CS_CLASS()
struct CS_CORE_API iDirectionalLight : public CS_SUPER(iLight)
{
  CS_CLASS_GEN;
  ~iDirectionalLight() override = default;

  virtual void SetDirection(const Vector3f & direction) = 0;
  CS_NODISCARD virtual const Vector3f& GetDirection() const = 0;


};

}