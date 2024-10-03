
#include "mirrorhandler.hh"


MirrorHandler::MirrorHandler()
: cryo::EntityState()
, m_cameraState(nullptr)
{
  SetNeedUpdate(true);
}

MirrorHandler::~MirrorHandler()
{
  CS_RELEASE(m_cameraState);
}

void MirrorHandler::SetCameraState(cryo::SpatialState *cameraState)
{
  CS_SET(m_cameraState, cameraState);
}


void MirrorHandler::Update(float tpf)
{
  const cryo::Matrix4f camMat = m_cameraState->GetGlobalMatrix();
  cryo::Vector3f pos = cryo::Matrix4f::Transform(camMat, cryo::Vector3f(0.0f, 0.0f, 0.0f));
  cryo::Vector3f dir = cryo::Matrix4f::Mult(camMat, cryo::Vector3f(0, 0, 1));
  cryo::Vector3f spot = pos + dir;


  GetRoot()->GetTransform()
  .SetTranslation(cryo::Vector3f(pos.x, -pos.y, pos.z))
  .LookAt(cryo::Vector3f(spot.x, -spot.y, spot.z), cryo::Vector3f(0.0f, 1.0f, 0.0f))
  .Finish();



}