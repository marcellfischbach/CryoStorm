#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/math/matrix4f.hh>
#include <ceCore/entity/spatialstate.hh>

namespace ce
{

struct iCollisionShape;

CE_CLASS(Virtual)
struct CE_CORE_API iCollider : public CE_SUPER(iObject)
{
  CE_CLASS_GEN;
  ~iCollider() override = default;


  virtual void Attach(iCollisionShape * shape) = 0;
  virtual void Detach(iCollisionShape * shape) = 0;


  virtual void SetTransform(const Matrix4f & transform) = 0;
  virtual const Matrix4f &GetTransform() const = 0;

};


CE_CLASS(Virtual)
struct CE_CORE_API iStaticCollider : public CE_SUPER(iCollider)
{
  CE_CLASS_GEN;
  ~iStaticCollider() override = default;

};

CE_CLASS(Virtual)
struct CE_CORE_API iDynamicCollider : public CE_SUPER(iCollider)
{
  CE_CLASS_GEN;
  ~iDynamicCollider() override = default;

  virtual void SetUserData(SpatialState * spatialState) = 0;
  virtual SpatialState *GetUserData() const = 0;

};


CE_CLASS(Virtual)
struct CE_CORE_API iTriggerCollider : public CE_SUPER(iCollider)
{
  CE_CLASS_GEN;
  ~iTriggerCollider() override = default;


};

}