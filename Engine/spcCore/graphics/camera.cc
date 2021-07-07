
#include <spcCore/graphics/camera.hh>
#include <spcCore/graphics/idevice.hh>


namespace spc
{


Camera::Camera()
  : iObject()
  , m_eye(0, 0, 0)
  , m_spot(0, 0, 1)
  , m_up(0, 1, 0)
{
  SPC_CLASS_GEN_CONSTR;
}

Camera::~Camera()
{

}



void Camera::SetEye(const Vector3f& eye)
{
  m_eye = eye;
}

const Vector3f& Camera::GetEye() const
{
  return m_eye;
}



void Camera::SetSpot(const Vector3f& spot)
{
  m_spot = spot;
}

const Vector3f& Camera::GetSpot() const
{
  return m_spot;
}


void Camera::SetUp(const Vector3f& up)
{
  m_up = up;
}

const Vector3f& Camera::GetUp() const
{
  return m_up;
}

void Camera::Bind(iDevice* device) const
{
  Matrix4f viewMatrix, viewMatrixInv;
  viewMatrix.SetLookAt(m_eye, m_spot, m_up);
  viewMatrixInv.SetLookAtInv(m_eye, m_spot, m_up);
  device->SetViewMatrix(viewMatrix, viewMatrixInv);
}


}