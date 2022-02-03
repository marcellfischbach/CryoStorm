
#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>

namespace spc
{

struct iPhysicsWorld;

SPC_CLASS()
struct SPC_CORE_API iPhysicsSystem : public SPC_SUPER(iObject)
{
  SPC_CLASS_GEN;

  ~iPhysicsSystem() override = default;


  SPC_NODISCARD virtual iPhysicsWorld *CreateWorld() const = 0;
};


}