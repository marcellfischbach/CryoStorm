#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/math/csMatrix4f.hh>
#include <csCore/entity/csSpatialState.hh>

namespace cryo
{

struct iCollisionShape;

CS_CLASS(Virtual)
struct CS_CORE_API iCollider : public CS_SUPER(iObject)
{
  CS_CLASS_GEN;
  ~iCollider() override = default;


  virtual void Attach(iCollisionShape * shape) = 0;
  virtual void Detach(iCollisionShape * shape) = 0;


  virtual void SetTransform(const csMatrix4f & transform) = 0;
  virtual const csMatrix4f &GetTransform() const = 0;

};


CS_CLASS(Virtual)
struct CS_CORE_API iStaticCollider : public CS_SUPER(iCollider)
{
  CS_CLASS_GEN;
  ~iStaticCollider() override = default;

};

CS_CLASS(Virtual)
struct CS_CORE_API iDynamicCollider : public CS_SUPER(iCollider)
{
  CS_CLASS_GEN;
  ~iDynamicCollider() override = default;

  virtual void SetUserData(csSpatialState * spatialState) = 0;
  virtual csSpatialState *GetUserData() const = 0;

};


CS_CLASS(Virtual)
struct CS_CORE_API iTriggerCollider : public CS_SUPER(iCollider)
{
  CS_CLASS_GEN;
  ~iTriggerCollider() override = default;


};

}