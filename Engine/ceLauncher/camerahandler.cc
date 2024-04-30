#include <ceLauncher/camerahandler.hh>
#include "ceCore/input/input.hh"
#include <ceCore/math/vector3f.hh>
#include <ceCore/entity/transform.hh>
#include <ceCore/entity/spatialstate.hh>

CameraHandler::CameraHandler()
        : ce::EntityState(), m_speed(4.0f), m_rotSpeed(0.0025f), m_rotY(1.88), m_rotX(-0.96)
{
  CE_CLASS_GEN_CONSTR;
  SetNeedUpdate(true);
}


void CameraHandler::OnAttachedToWorld(ce::World *world)
{
  EntityState::OnAttachedToWorld(world);
  ce::Input::GetMouse()->SetCursorMode(ce::eCursorMode::Fixed);
}

void CameraHandler::Update(float tpf)
{
  ce::Transform tr = GetRoot()->GetTransform();
  ce::Vector3f  dir;

  if (ce::Input::IsKeyDown(ce::Key::eK_W))
  {
    dir += tr.GetForward() * tpf * m_speed;
  }
  if (ce::Input::IsKeyDown(ce::Key::eK_S))
  {
    dir += tr.GetBackward() * tpf * m_speed;
  }
  if (ce::Input::IsKeyDown(ce::Key::eK_A))
  {
    dir += tr.GetLeft() * tpf * m_speed;
  }
  if (ce::Input::IsKeyDown(ce::Key::eK_D))
  {
    dir += tr.GetRight() * tpf * m_speed;
  }

  if (ce::Input::IsKeyDown(ce::Key::eK_LeftShift) ||
      ce::Input::IsKeyDown(ce::Key::eK_RightShift))
  {
    dir *= 2.0f;
  }

  const ce::Vector2f &mouse = ce::Input::GetMouseDelta();
  m_rotX += -mouse.y * m_rotSpeed;
  m_rotY += -mouse.x * m_rotSpeed;

  float clamp = (float) M_PI * 0.48f;
  float pi2   = (float) M_PI * 2.0f;
  m_rotX = ce::ceClamp(m_rotX, -clamp, clamp);

  while (m_rotY > pi2)
  {
    m_rotY -= pi2;
  }
  while (m_rotY < 0)
  {
    m_rotY += pi2;
  }

  ce::Quaternion rotY = ce::Quaternion::FromAxisAngle(0.0f, 1.0f, 0.0f, m_rotY);
  ce::Quaternion rotX = ce::Quaternion::FromAxisAngle(1.0f, 0.0f, 0.0f, m_rotX);
  ce::Quaternion rot  = rotX * rotY;


  tr.SetTranslation(tr.GetTranslation() + dir)
    .SetRotation(rot)
    .Finish();
//  tr.SetTranslation(ce::Vector3f(5.0f, 10.0f, 5.0f))
//    .LookAt(ce::Vector3f(0.0f, 0.0f, 0.0f))
//    .Finish();
}








CameraHandlerMotion::CameraHandlerMotion()
    : ce::EntityState(), m_position(5.0f, 5.0f, 5.0f), m_target(0, 0, 0), m_distance(1.0f), m_time(0.0f)
{
  CE_CLASS_GEN_CONSTR;
  SetNeedUpdate(true);
}



void CameraHandlerMotion::Update(float tpf)
{
  ce::Transform tr = GetRoot()->GetTransform();

  m_time += tpf;
  m_time = ::fmod(m_time, (3.14156f * 2.0f));
  ce::Vector3f pos = m_position;
  pos.y += (::sinf(m_time) * 0.5f + 0.5f) * m_distance;
  tr.SetTranslation(pos)
    .LookAt(m_target)
    .Finish();
//  tr.SetTranslation(ce::Vector3f(5.0f, 10.0f, 5.0f))
//    .LookAt(ce::Vector3f(0.0f, 0.0f, 0.0f))
//    .Finish();
}
