
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/entity/entitystate.hh>

namespace ce
{

CE_CLASS(jclass = "org.crimsonedge.core.entity.JavaState")
class CE_CORE_API JavaState : public CE_SUPER(EntityState)
{
CE_CLASS_GEN;
public:
  JavaState();
  ~JavaState() override = default;


  void OnAttachedToWorld(World* world) override;
  void OnDetachedFromWorld(World* world) override;

  void Update(float tpf) override;
};

}