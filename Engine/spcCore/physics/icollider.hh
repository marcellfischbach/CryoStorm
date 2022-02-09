#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>
#include <spcCore/math/matrix4f.hh>
#include <spcCore/entity/spatialstate.hh>

namespace spc
{

struct iCollisionShape;

SPC_CLASS(Virtual)
struct SPC_CORE_API iCollider : public SPC_SUPER(iObject)
{
  SPC_CLASS_GEN;
  ~iCollider() override = default;


  virtual void Attach(iCollisionShape * shape) = 0;
  virtual void Detach(iCollisionShape * shape) = 0;


  virtual void SetTransform(const Matrix4f & transform) = 0;
  virtual const Matrix4f &GetTransform() const = 0;

};


SPC_CLASS(Virtual)
struct SPC_CORE_API iStaticCollider : public SPC_SUPER(iCollider)
{
  SPC_CLASS_GEN;
  ~iStaticCollider() override = default;

};

SPC_CLASS(Virtual)
struct SPC_CORE_API iDynamicCollider : public SPC_SUPER(iCollider)
{
  SPC_CLASS_GEN;
  ~iDynamicCollider() override = default;

  virtual void SetUserData(SpatialState * spatialState) = 0;
  virtual SpatialState *GetUserData() const = 0;

};


SPC_CLASS(Virtual)
struct SPC_CORE_API iTriggerCollider : public SPC_SUPER(iCollider)
{
  SPC_CLASS_GEN;
  ~iTriggerCollider() override = default;


};

}