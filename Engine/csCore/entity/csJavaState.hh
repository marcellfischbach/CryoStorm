
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/entity/csEntityState.hh>

namespace cs
{

CS_CLASS(jclass = "org.cryo.core.entity.CsJavaState")
class CS_CORE_API csJavaState : public CS_SUPER(csEntityState)
{
CS_CLASS_GEN;
public:
  csJavaState();
  ~csJavaState() override;


  void OnAttachedToWorld(csWorld* world) override;
  void OnDetachedFromWorld(csWorld* world) override;

  void Update(float tpf) override;

private:
  struct  csJavaStatePrivate *m_priv;
};

}