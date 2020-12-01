

#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>
#include <spcCore/math/vector3f.hh>

namespace spc
{

struct iDevice;

SPC_CLASS()
class SPC_CORE_API Camera : public SPC_SUPER(iObject)
{
  SPC_CLASS_GEN_OBJECT;
public:
  Camera();
  virtual ~Camera();

  void SetEye(const Vector3f & eye);
  const Vector3f &GetEye() const;

  void SetSpot(const Vector3f & spot);
  const Vector3f &GetSpot() const;

  void SetUp(const Vector3f & up);
  const Vector3f &GetUp() const;


  void Bind(iDevice * device);

private:

  Vector3f m_eye;
  Vector3f m_spot;
  Vector3f m_up;

};


}
