#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/math/matrix4f.hh>
#include <vector>

namespace ce
{


struct iStaticCollider;
struct iDynamicCollider;



CE_CLASS()
struct CE_CORE_API iPhysicsWorld : public CE_SUPER(iObject)
{
  CE_CLASS_GEN;
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