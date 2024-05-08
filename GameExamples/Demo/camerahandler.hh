
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



CE_CLASS()
class CameraHandlerMotion : public CE_SUPER(ce::EntityState)
{
  CE_CLASS_GEN_OBJECT;
public:
  CameraHandlerMotion();
  ~CameraHandlerMotion() override = default;

  void Update(float tpf) override;
private:
  ce::Vector3f m_position;
  ce::Vector3f m_target;
  float m_distance;
  float m_time;
};
