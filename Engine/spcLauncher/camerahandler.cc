#include <spcLauncher/camerahandler.hh>
#include <spcCore/input/input.hh>
#include <spcCore/math/vector3f.hh>
#include <spcCore/entity/transform.hh>
#include <spcCore/entity/spatialstate.hh>

CameraHandler::CameraHandler()
    : spc::EntityState(), m_speed(1.0f)
{
  SPC_CLASS_GEN_CONSTR;
  SetNeedUpdate(true);
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

  tr.SetTranslation(tr.GetTranslation() + dir)
    .Finish();
}