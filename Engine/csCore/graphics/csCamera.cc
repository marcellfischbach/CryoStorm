
#include <csCore/graphics/csCamera.hh>
#include <csCore/graphics/iDevice.hh>


namespace cs
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



void csCamera::SetEye(const csVector3f& eye)
{
  m_eye = eye;
}

const csVector3f& csCamera::GetEye() const
{
  return m_eye;
}



void csCamera::SetSpot(const csVector3f& spot)
{
  m_spot = spot;
}

const csVector3f& csCamera::GetSpot() const
{
  return m_spot;
}


void csCamera::SetUp(const csVector3f& up)
{
  m_up = up;
}

const csVector3f& csCamera::GetUp() const
{
  return m_up;
}

void csCamera::Bind(iDevice* device) const
{
  csMatrix4f viewMatrix, viewMatrixInv;
  viewMatrix.SetLookAt(m_eye, m_spot, m_up);
  viewMatrixInv.SetLookAtInv(m_eye, m_spot, m_up);
  device->SetViewMatrix(viewMatrix, viewMatrixInv);
}

csMatrix4f csCamera::GetViewMatrix() const
{
  csMatrix4f matrix;
  matrix.SetLookAt(m_eye, m_spot, m_up);
  return matrix;
}



csMatrix4f csCamera::GetViewMatrixInv() const
{
  csMatrix4f matrix;
  matrix.SetLookAtInv(m_eye, m_spot, m_up);
  return matrix;
}

}