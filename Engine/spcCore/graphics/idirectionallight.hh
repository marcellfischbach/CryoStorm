

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
  virtual SPC_NODISCARD const Vector3f& GetDirection() const = 0;

  virtual void SetSplits(float split0, float split1, float split2) = 0;
  virtual SPC_NODISCARD float GetSplit0() const = 0;
  virtual SPC_NODISCARD float GetSplit1() const = 0;
  virtual SPC_NODISCARD float GetSplit2() const = 0;

};

}