#include <spcLauncher/camerahandler.hh>
#include <spcCore/input/input.hh>
#include <spcCore/math/vector3f.hh>
#include <spcCore/entity/transform.hh>
#include <spcCore/entity/spatialstate.hh>

CameraHandler::CameraHandler()
    : spc::EntityState(), m_speed(4.0f), m_rotSpeed(0.0025f)
{
  SPC_CLASS_GEN_CONSTR;
  SetNeedUpdate(true);
}


void CameraHandler::OnAttachedToWorld(spc::World *world)
{
  EntityState::OnAttachedToWorld(world);
  spc::Input::GetMouse()->SetCursorMode(spc::eCursorMode::Fixed);
}

void CameraHandler::Update(float tpf)
{
  spc::Transform tr = GetRoot()->GetTransform();
  spc::Vector3f  dir;

  if (spc::Input::IsKeyDown(spc::Key::eK_W))
  {
    dir += tr.GetForward() * tpf * m_speed;
  }
  if (spc::Input::IsKeyDown(spc::Key::eK_S))
  {
    dir += tr.GetBackward() * tpf * m_speed;
  }
  if (spc::Input::IsKeyDown(spc::Key::eK_A))
  {
    dir += tr.GetLeft() * tpf * m_speed;
  }
  if (spc::Input::IsKeyDown(spc::Key::eK_D))
  {
    dir += tr.GetRight() * tpf * m_speed;
  }

  if (spc::Input::IsKeyDown(spc::Key::eK_LeftShift) ||
      spc::Input::IsKeyDown(spc::Key::eK_RightShift))
  {
    dir *= 2.0f;
  }

  const spc::Vector2f &mouse = spc::Input::GetMouseDelta();
  m_rotX += -mouse.y * m_rotSpeed;
  m_rotY += -mouse.x * m_rotSpeed;

  float clamp = (float) M_PI * 0.48f;
  float pi2   = (float) M_PI * 2.0f;
  m_rotX = spc::spcClamp(m_rotX, -clamp, clamp);

  while (m_rotY > pi2)
  {
    m_rotY -= pi2;
  }
  while (m_rotY < 0)
  {
    m_rotY += pi2;
  }

  spc::Quaternion rotY = spc::Quaternion::FromAxisAngle(0.0f, 1.0f, 0.0f, m_rotY);
  spc::Quaternion rotX = spc::Quaternion::FromAxisAngle(1.0f, 0.0f, 0.0f, m_rotX);
  spc::Quaternion rot  = rotX * rotY;

  tr.SetTranslation(tr.GetTranslation() + dir)
    .SetRotation(rot)
    .Finish();
}
