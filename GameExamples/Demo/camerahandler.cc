
#include "camerahandler.hh"
#include <ceCore/input/input.hh>
#include <ceCore/math/vector3f.hh>
#include <ceCore/entity/csTransform.hh>
#include <ceCore/entity/csSpatialState.hh>

CameraHandler::CameraHandler()
//        : cryo::EntityState(), m_speed(4.0f), m_rotSpeed(0.0025f), m_rotY(1.88), m_rotX(-0.96)
        : cryo::csEntityState(), m_speed(4.0f), m_rotSpeed(0.0025f), m_rotY(4.02), m_rotX(0.33)
{
  CS_CLASS_GEN_CONSTR;
  SetNeedUpdate(true);
}


void CameraHandler::OnAttachedToWorld(cryo::csWorld *world)
{
  csEntityState::OnAttachedToWorld(world);
//  cryo::Input::GetMouse()->SetCursorMode(cryo::eCursorMode::eCM_Fixed);
}

void CameraHandler::Update(float tpf)
{
  cryo::csTransform tr = GetRoot()->GetTransform();
  cryo::Vector3f    dir;



  cryo::iMouse         *pMouse = cryo::Input::GetMouse();
  if (pMouse->IsButtonDown(cryo::eMB_Right))
  {
    if (cryo::Input::IsKeyDown(cryo::Key::eK_W))
    {
      dir += tr.GetForward() * tpf * m_speed;
    }
    if (cryo::Input::IsKeyDown(cryo::Key::eK_S))
    {
      dir += tr.GetBackward() * tpf * m_speed;
    }
    if (cryo::Input::IsKeyDown(cryo::Key::eK_A))
    {
      dir += tr.GetLeft() * tpf * m_speed;
    }
    if (cryo::Input::IsKeyDown(cryo::Key::eK_D))
    {
      dir += tr.GetRight() * tpf * m_speed;
    }

    if (cryo::Input::IsKeyDown(cryo::Key::eK_LeftShift) ||
        cryo::Input::IsKeyDown(cryo::Key::eK_RightShift))
    {
      dir *= 2.0f;
    }

    pMouse->SetCursorMode(cryo::eCM_Fixed);
    pMouse->SetVisible(false);
    const cryo::Vector2f &mouse = cryo::Input::GetMouseDelta();
    m_rotX += -mouse.y * m_rotSpeed;
    m_rotY += -mouse.x * m_rotSpeed;

    float clamp = (float) M_PI * 0.48f;
    float pi2   = (float) M_PI * 2.0f;
    m_rotX = cryo::ceClamp(m_rotX, -clamp, clamp);

    while (m_rotY > pi2)
    {
      m_rotY -= pi2;
    }
    while (m_rotY < 0)
    {
      m_rotY += pi2;
    }
  }
  else {
    pMouse->SetCursorMode(cryo::eCM_Free);
    pMouse->SetVisible(true);

  }
  cryo::Quaternion rotY = cryo::Quaternion::FromAxisAngle(0.0f, 1.0f, 0.0f, m_rotY);
  cryo::Quaternion rotX = cryo::Quaternion::FromAxisAngle(1.0f, 0.0f, 0.0f, m_rotX);
  cryo::Quaternion rot  = rotX * rotY;


  tr.SetTranslation(tr.GetTranslation() + dir)
    .SetRotation(rot)
    .Finish();
//  tr.SetTranslation(cryo::Vector3f(5.0f, 10.0f, 5.0f))
//    .LookAt(cryo::Vector3f(0.0f, 0.0f, 0.0f))
//    .Finish();

}








CameraHandlerMotion::CameraHandlerMotion()
    : cryo::csEntityState(), m_position(5.0f, 5.0f, 5.0f), m_target(0, 0, 0), m_distance(1.0f), m_time(0.0f)
{
  CS_CLASS_GEN_CONSTR;
  SetNeedUpdate(true);
}



void CameraHandlerMotion::Update(float tpf)
{
  cryo::csTransform tr = GetRoot()->GetTransform();

  m_time += tpf;
  m_time = ::fmod(m_time, (3.14156f * 2.0f));
  cryo::Vector3f pos = m_position;
  pos.y += (::sinf(m_time) * 0.5f + 0.5f) * m_distance;
  tr.SetTranslation(pos)
    .LookAt(m_target)
    .Finish();
//  tr.SetTranslation(cryo::Vector3f(5.0f, 10.0f, 5.0f))
//    .LookAt(cryo::Vector3f(0.0f, 0.0f, 0.0f))
//    .Finish();
}
