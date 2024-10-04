

#pragma once

#include <ceCore/csCoreExport.hh>
#include <ceCore/graphics/iLight.hh>
#include <ceCore/math/csVector3f.hh>


namespace cryo
{


CS_CLASS()
struct CS_CORE_API iDirectionalLight : public CS_SUPER(iLight)
{
  CS_CLASS_GEN;
  ~iDirectionalLight() override = default;

  virtual void SetDirection(const csVector3f & direction) = 0;
  CS_NODISCARD virtual const csVector3f& GetDirection() const = 0;


};

}