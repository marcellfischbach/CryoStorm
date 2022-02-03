#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>


namespace spc
{


SPC_CLASS()
struct SPC_CORE_API iPhysicsWorld : public SPC_SUPER(iObject)
{
  SPC_CLASS_GEN;
  ~iPhysicsWorld() override = default;


  virtual void Step(float tpf) = 0;
};


}