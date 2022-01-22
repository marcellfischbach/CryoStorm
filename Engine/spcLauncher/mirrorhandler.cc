
#include <spcLauncher/mirrorhandler.hh>


MirrorHandler::MirrorHandler()
: spc::EntityState()
, m_cameraState(nullptr)
{
  SetNeedUpdate(true);
}

void MirrorHandler::SetCameraState(spc::SpatialState *cameraState)
{
  SPC_SET(m_cameraState, cameraState);
}


void MirrorHandler::Update(float tpf)
{
  const spc::Matrix4f camMat = m_cameraState->GetGlobalMatrix();
  spc::Vector3f pos = spc::Matrix4f::Transform(camMat, spc::Vector3f(0.0f, 0.0f, 0.0f));
  spc::Vector3f dir = spc::Matrix4f::Mult(camMat, spc::Vector3f(0, 0, 1));
  spc::Vector3f spot = pos + dir;


  GetRoot()->GetTransform()
  .SetTranslation(spc::Vector3f(pos.x, -pos.y, pos.z))
  .LookAt(spc::Vector3f(spot.x, -spot.y, spot.z), spc::Vector3f(0.0f, 1.0f, 0.0f))
  .Finish();



}