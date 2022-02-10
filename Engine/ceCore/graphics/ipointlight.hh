



#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/graphics/ilight.hh>
#include <ceCore/math/vector3f.hh>


namespace ce
{


CE_CLASS()
struct CE_CORE_API iPointLight : public CE_SUPER(iLight)
{
  CE_CLASS_GEN;
  virtual ~iPointLight() { }

  virtual void SetPosition(const Vector3f & direction) = 0;
  virtual CE_NODISCARD const Vector3f& GetPosition() const = 0;

  virtual void SetRange(float range) = 0;
  virtual CE_NODISCARD float GetRange() const = 0;


};

}