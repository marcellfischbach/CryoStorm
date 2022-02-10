

#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/graphics/ilight.hh>
#include <ceCore/math/vector3f.hh>


namespace ce
{


CE_CLASS()
struct CE_CORE_API iDirectionalLight : public CE_SUPER(iLight)
{
  CE_CLASS_GEN;
  ~iDirectionalLight() override = default;

  virtual void SetDirection(const Vector3f & direction) = 0;
  CE_NODISCARD virtual const Vector3f& GetDirection() const = 0;


};

}