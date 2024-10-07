

#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/math/csMatrix4f.hh>
#include <csCore/math/csVector3f.hh>

namespace cs
{

struct iDevice;

CS_CLASS()
class CS_CORE_API csCamera : public CS_SUPER(iObject)
{
  CS_CLASS_GEN_OBJECT;
public:
  csCamera();
  virtual ~csCamera();

  void SetEye(const csVector3f & eye);
  const csVector3f &GetEye() const;

  void SetSpot(const csVector3f & spot);
  const csVector3f &GetSpot() const;

  void SetUp(const csVector3f & up);
  const csVector3f &GetUp() const;


  void Bind(iDevice * device) const;
  csMatrix4f GetViewMatrix() const;
  csMatrix4f GetViewMatrixInv() const;

private:

  csVector3f m_eye;
  csVector3f m_spot;
  csVector3f m_up;

};


}
