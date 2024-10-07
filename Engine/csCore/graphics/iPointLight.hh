



#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/graphics/iLight.hh>
#include <csCore/math/csVector3f.hh>


namespace cs
{


CS_CLASS()
struct CS_CORE_API iPointLight : public CS_SUPER(iLight)
{
  CS_CLASS_GEN;
  virtual ~iPointLight() { }

  virtual void SetPosition(const csVector3f & direction) = 0;
  virtual CS_NODISCARD const csVector3f& GetPosition() const = 0;

  virtual void SetRange(float range) = 0;
  virtual CS_NODISCARD float GetRange() const = 0;


};

}