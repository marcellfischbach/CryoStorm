
#pragma once

#include <csCore/entity/csEntitySystem.hh>

CS_CLASS()
class CameraHandler : public CS_SUPER(cryo::csEntityState)
{
CS_CLASS_GEN_OBJECT;
public:
  CameraHandler();
  ~CameraHandler() override = default;

  void OnAttachedToWorld(cryo::csWorld *world) override;
  void Update(float tpf) override;
private:
  float m_speed;
  float m_rotSpeed;
  float m_rotX;
  float m_rotY;
};



CS_CLASS()
class CameraHandlerMotion : public CS_SUPER(cryo::csEntityState)
{
  CS_CLASS_GEN_OBJECT;
public:
  CameraHandlerMotion();
  ~CameraHandlerMotion() override = default;

  void Update(float tpf) override;
private:
  cryo::csVector3f m_position;
  cryo::csVector3f m_target;
  float            m_distance;
  float m_time;
};
