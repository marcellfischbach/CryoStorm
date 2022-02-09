#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>
#include <spcCore/math/matrix4f.hh>
#include <vector>

namespace spc
{


struct iStaticCollider;
struct iDynamicCollider;



SPC_CLASS()
struct SPC_CORE_API iPhysicsWorld : public SPC_SUPER(iObject)
{
  SPC_CLASS_GEN;
  ~iPhysicsWorld() override = default;

  struct DynamicResult
  {
    iDynamicCollider* Collider;
    Matrix4f          Matrix;
  };

  virtual void Step(float tpf) = 0;
  virtual const std::vector<DynamicResult>& SwapResult() = 0;


  virtual void AddCollider(iStaticCollider * collider) = 0;
  virtual void AddCollider(iDynamicCollider * collider) = 0;


  virtual void RemoveCollider(iStaticCollider * collider) = 0;
  virtual void RemoveCollider(iDynamicCollider * collider) = 0;

};


}