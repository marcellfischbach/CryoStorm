
#pragma once

#include <spcCore/entity/entitystate.hh>

SPC_CLASS()
class CameraHandler : public SPC_SUPER(spc::EntityState)
{
SPC_CLASS_GEN_OBJECT;
public:
  CameraHandler();
  ~CameraHandler() override = default;

  void Update(float tpf) override;
private:
  float m_speed;
};



