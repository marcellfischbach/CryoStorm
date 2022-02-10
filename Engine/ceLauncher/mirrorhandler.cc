
#include <ceLauncher/mirrorhandler.hh>


MirrorHandler::MirrorHandler()
: ce::EntityState()
, m_cameraState(nullptr)
{
  SetNeedUpdate(true);
}

MirrorHandler::~MirrorHandler()
{
  CE_RELEASE(m_cameraState);
}

void MirrorHandler::SetCameraState(ce::SpatialState *cameraState)
{
  CE_SET(m_cameraState, cameraState);
}


void MirrorHandler::Update(float tpf)
{
  const ce::Matrix4f camMat = m_cameraState->GetGlobalMatrix();
  ce::Vector3f pos = ce::Matrix4f::Transform(camMat, ce::Vector3f(0.0f, 0.0f, 0.0f));
  ce::Vector3f dir = ce::Matrix4f::Mult(camMat, ce::Vector3f(0, 0, 1));
  ce::Vector3f spot = pos + dir;


  GetRoot()->GetTransform()
  .SetTranslation(ce::Vector3f(pos.x, -pos.y, pos.z))
  .LookAt(ce::Vector3f(spot.x, -spot.y, spot.z), ce::Vector3f(0.0f, 1.0f, 0.0f))
  .Finish();



}