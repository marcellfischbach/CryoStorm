#pragma once

#include <ceCore/csCoreExport.hh>
#include <ceCore/csClass.hh>
#include <ceCore/math/csMatrix4f.hh>
#include <vector>

namespace cryo
{


struct iStaticCollider;
struct iDynamicCollider;



CS_CLASS()
struct CS_CORE_API iPhysicsWorld : public CS_SUPER(iObject)
{
  CS_CLASS_GEN;
  ~iPhysicsWorld() override = default;

  struct DynamicResult
  {
    iDynamicCollider* Collider;
    csMatrix4f Matrix;
  };

  virtual void Step(float tpf) = 0;
  virtual const std::vector<DynamicResult>& SwapResult() = 0;


  virtual void AddCollider(iStaticCollider * collider) = 0;
  virtual void AddCollider(iDynamicCollider * collider) = 0;


  virtual void RemoveCollider(iStaticCollider * collider) = 0;
  virtual void RemoveCollider(iDynamicCollider * collider) = 0;

};


}