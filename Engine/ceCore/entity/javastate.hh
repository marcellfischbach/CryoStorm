
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/entity/entitystate.hh>

namespace cryo
{

CS_CLASS(jclass = "org.crimsonedge.core.entity.JavaState")
class CS_CORE_API JavaState : public CS_SUPER(EntityState)
{
CS_CLASS_GEN;
public:
  JavaState();
  ~JavaState() override = default;


  void OnAttachedToWorld(World* world) override;
  void OnDetachedFromWorld(World* world) override;

  void Update(float tpf) override;
};

}