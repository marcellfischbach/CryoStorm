
#include "mirrorhandler.hh"


MirrorHandler::MirrorHandler()
: cs::csEntityState()
, m_cameraState(nullptr)
{
  SetNeedUpdate(true);
}

MirrorHandler::~MirrorHandler()
{
  CS_RELEASE(m_cameraState);
}

void MirrorHandler::SetCameraState(cs::csSpatialState *cameraState)
{
  CS_SET(m_cameraState, cameraState);
}


void MirrorHandler::Update(float tpf)
{
  const cs::csMatrix4f camMat = m_cameraState->GetGlobalMatrix();
  cs::csVector3f       pos    = cs::csMatrix4f::Transform(camMat, cs::csVector3f(0.0f, 0.0f, 0.0f));
  cs::csVector3f       dir    = cs::csMatrix4f::Mult(camMat, cs::csVector3f(0, 0, 1));
  cs::csVector3f       spot   = pos + dir;


  GetRoot()->GetTransform()
  .SetTranslation(cs::csVector3f(pos.x, -pos.y, pos.z))
  .LookAt(cs::csVector3f(spot.x, -spot.y, spot.z), cs::csVector3f(0.0f, 1.0f, 0.0f))
  .Finish();



}