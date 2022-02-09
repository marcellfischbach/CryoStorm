
#pragma once


#include <spcCore/coreexport.hh>
#include <spcCore/entity/spatialstate.hh>

namespace spc
{

SPC_CLASS()
class SPC_CORE_API CollisionState : public SPC_SUPER(SpatialState)
{
  SPC_CLASS_GEN_OBJECT;
public:
  CollisionState();
  ~CollisionState() override;

};


}
