
#include "camerahandler.hh"
#include <csCore/input/csInput.hh>
#include <csCore/math/csVector3f.hh>
#include <csCore/entity/csTransform.hh>
#include <csCore/entity/csSpatialState.hh>
#include <csCore/graphics/csSkeleton.hh>

using namespace cs;

CameraHandler::CameraHandler()
//        : EntityState(), m_speed(4.0f), m_rotSpeed(0.0025f), m_rotY(1.88), m_rotX(-0.96)
        : csEntityState(), m_speed(4.0f), m_rotSpeed(0.0025f), m_rotY(0.46), m_rotX(-0.58)
{

  SetNeedUpdate(true);
}


void CameraHandler::OnAttachedToWorld(csWorld *world)
{
  csEntityState::OnAttachedToWorld(world);
//  Input::GetMouse()->SetCursorMode(eCursorMode::eCM_Fixed);
}

void CameraHandler::Update(float tpf)
{
  csTransform tr = GetRoot()->GetTransform();
  csVector3f  dir;



  iMouse         *pMouse = csInput::GetMouse();
  if (pMouse->IsButtonDown(eMB_Right))
  {
    if (csInput::IsKeyDown(eKey::eK_W))
    {
      dir += tr.GetForward() * tpf * m_speed;
    }
    if (csInput::IsKeyDown(eKey::eK_S))
    {
      dir += tr.GetBackward() * tpf * m_speed;
    }
    if (csInput::IsKeyDown(eKey::eK_A))
    {
      dir += tr.GetLeft() * tpf * m_speed;
    }
    if (csInput::IsKeyDown(eKey::eK_D))
    {
      dir += tr.GetRight() * tpf * m_speed;
    }

    if (csInput::IsKeyDown(eKey::eK_LeftShift) ||
        csInput::IsKeyDown(eKey::eK_RightShift))
    {
      dir *= 2.0f;
    }

    pMouse->SetCursorMode(eCM_Fixed);
    pMouse->SetVisible(false);
    const csVector2f &mouse = csInput::GetMouseDelta();
    m_rotX += -mouse.y * m_rotSpeed;
    m_rotY += -mouse.x * m_rotSpeed;

    float clamp = (float) M_PI * 0.48f;
    float pi2   = (float) M_PI * 2.0f;
    m_rotX = ceClamp(m_rotX, -clamp, clamp);

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
    pMouse->SetCursorMode(eCM_Free);
    pMouse->SetVisible(true);

  }
  csQuaternion rotY = csQuaternion::FromAxisAngle(0.0f, 1.0f, 0.0f, m_rotY);
  csQuaternion rotX = csQuaternion::FromAxisAngle(1.0f, 0.0f, 0.0f, m_rotX);
  csQuaternion rot  = rotX * rotY;


  if (csInput::IsKeyPressed(eKey::eK_P))
  {
    printf ("(%.2f %.2f %.2f) @ (%.2f %.2f)\n",
            tr.GetTranslation().x,
            tr.GetTranslation().y,
            tr.GetTranslation().z,
            m_rotX,
            m_rotY
            );
    fflush(stdout);
  }

  tr.SetTranslation(tr.GetTranslation() + dir)
    .SetRotation(rot)
    .Finish();
//  tr.SetTranslation(Vector3f(5.0f, 10.0f, 5.0f))
//    .LookAt(Vector3f(0.0f, 0.0f, 0.0f))
//    .Finish();

  const csVector3f &t = dir; //tr.GetTranslation();
}








CameraHandlerMotion::CameraHandlerMotion()
    : csEntityState(), m_position(2.5f, 2.5f, 2.5f), m_target(0, 0, 0), m_distance(.250f), m_time(0.0f)
{

  SetNeedUpdate(true);
}



void CameraHandlerMotion::Update(float tpf)
{
  csTransform tr = GetRoot()->GetTransform();

  m_time += tpf;
  m_time = ::fmod(m_time, (3.14156f * 2.0f));
  csVector3f pos = m_position;
  pos.y += (::sinf(m_time) * 0.5f + 0.5f) * m_distance;
  tr.SetTranslation(pos)
    .LookAt(m_target)
    .Finish();
//  tr.SetTranslation(Vector3f(5.0f, 10.0f, 5.0f))
//    .LookAt(Vector3f(0.0f, 0.0f, 0.0f))
//    .Finish();
}
