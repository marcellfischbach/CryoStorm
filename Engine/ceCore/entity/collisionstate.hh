
#pragma once


#include <ceCore/coreexport.hh>
#include <ceCore/entity/spatialstate.hh>

namespace ce
{

CE_CLASS()
class CE_CORE_API CollisionState : public CE_SUPER(SpatialState)
{
  CE_CLASS_GEN_OBJECT;
public:
  CollisionState();
  ~CollisionState() override;

};


}
