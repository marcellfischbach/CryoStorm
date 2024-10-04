
#include "mirrorhandler.hh"


MirrorHandler::MirrorHandler()
: cryo::csEntityState()
, m_cameraState(nullptr)
{
  SetNeedUpdate(true);
}

MirrorHandler::~MirrorHandler()
{
  CS_RELEASE(m_cameraState);
}

void MirrorHandler::SetCameraState(cryo::csSpatialState *cameraState)
{
  CS_SET(m_cameraState, cameraState);
}


void MirrorHandler::Update(float tpf)
{
  const cryo::csMatrix4f camMat = m_cameraState->GetGlobalMatrix();
  cryo::csVector3f       pos    = cryo::csMatrix4f::Transform(camMat, cryo::csVector3f(0.0f, 0.0f, 0.0f));
  cryo::csVector3f       dir    = cryo::csMatrix4f::Mult(camMat, cryo::csVector3f(0, 0, 1));
  cryo::csVector3f       spot   = pos + dir;


  GetRoot()->GetTransform()
  .SetTranslation(cryo::csVector3f(pos.x, -pos.y, pos.z))
  .LookAt(cryo::csVector3f(spot.x, -spot.y, spot.z), cryo::csVector3f(0.0f, 1.0f, 0.0f))
  .Finish();



}