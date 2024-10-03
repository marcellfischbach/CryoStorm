
#include <ceCore/graphics/csCamera.hh>
#include <ceCore/graphics/iDevice.hh>


namespace cryo
{


csCamera::csCamera()
  : iObject()
  , m_eye(0, 0, 0)
  , m_spot(0, 0, 1)
  , m_up(0, 1, 0)
{
  CS_CLASS_GEN_CONSTR;
}

csCamera::~csCamera()
{

}



void csCamera::SetEye(const Vector3f& eye)
{
  m_eye = eye;
}

const Vector3f& csCamera::GetEye() const
{
  return m_eye;
}



void csCamera::SetSpot(const Vector3f& spot)
{
  m_spot = spot;
}

const Vector3f& csCamera::GetSpot() const
{
  return m_spot;
}


void csCamera::SetUp(const Vector3f& up)
{
  m_up = up;
}

const Vector3f& csCamera::GetUp() const
{
  return m_up;
}

void csCamera::Bind(iDevice* device) const
{
  Matrix4f viewMatrix, viewMatrixInv;
  viewMatrix.SetLookAt(m_eye, m_spot, m_up);
  viewMatrixInv.SetLookAtInv(m_eye, m_spot, m_up);
  device->SetViewMatrix(viewMatrix, viewMatrixInv);
}

Matrix4f csCamera::GetViewMatrix() const
{
  Matrix4f matrix;
  matrix.SetLookAt(m_eye, m_spot, m_up);
  return matrix;
}



Matrix4f csCamera::GetViewMatrixInv() const
{
  Matrix4f matrix;
  matrix.SetLookAtInv(m_eye, m_spot, m_up);
  return matrix;
}

}