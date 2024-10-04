
#pragma once

#include <ceCore/csCoreExport.hh>
#include <ceCore/entity/csEntityState.hh>

namespace cryo
{

CS_CLASS(jclass = "org.crimsonedge.core.entity.JavaState")
class CS_CORE_API csJavaState : public CS_SUPER(csEntityState)
{
CS_CLASS_GEN;
public:
  csJavaState();
  ~csJavaState() override = default;


  void OnAttachedToWorld(csWorld* world) override;
  void OnDetachedFromWorld(csWorld* world) override;

  void Update(float tpf) override;
};

}