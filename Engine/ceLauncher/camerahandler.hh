
#pragma once

#include <ceCore/entity/entitysystem.hh>

CE_CLASS()
class CameraHandler : public CE_SUPER(ce::EntityState)
{
CE_CLASS_GEN_OBJECT;
public:
  CameraHandler();
  ~CameraHandler() override = default;

  void OnAttachedToWorld(ce::World *world) override;
  void Update(float tpf) override;
private:
  float m_speed;
  float m_rotSpeed;
  float m_rotX;
  float m_rotY;
};



