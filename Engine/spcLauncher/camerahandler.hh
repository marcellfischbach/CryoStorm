
#pragma once

#include <spcCore/entity/entitysystem.hh>

SPC_CLASS()
class CameraHandler : public SPC_SUPER(spc::EntityState)
{
SPC_CLASS_GEN_OBJECT;
public:
  CameraHandler();
  ~CameraHandler() override = default;

  void OnAttachedToWorld(spc::World *world) override;
  void Update(float tpf) override;
private:
  float m_speed;
  float m_rotSpeed;
  float m_rotX;
  float m_rotY;
};



