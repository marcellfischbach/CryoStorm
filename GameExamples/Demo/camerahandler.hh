
#pragma once

#include <ceCore/entity/entitysystem.hh>

CS_CLASS()
class CameraHandler : public CS_SUPER(cryo::EntityState)
{
CS_CLASS_GEN_OBJECT;
public:
  CameraHandler();
  ~CameraHandler() override = default;

  void OnAttachedToWorld(cryo::World *world) override;
  void Update(float tpf) override;
private:
  float m_speed;
  float m_rotSpeed;
  float m_rotX;
  float m_rotY;
};



CS_CLASS()
class CameraHandlerMotion : public CS_SUPER(cryo::EntityState)
{
  CS_CLASS_GEN_OBJECT;
public:
  CameraHandlerMotion();
  ~CameraHandlerMotion() override = default;

  void Update(float tpf) override;
private:
  cryo::Vector3f m_position;
  cryo::Vector3f m_target;
  float m_distance;
  float m_time;
};
