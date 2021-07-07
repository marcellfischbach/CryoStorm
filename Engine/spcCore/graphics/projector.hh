

#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/graphics/eprojectionmode.hh>
#include <spcCore/math/vector3f.hh>

namespace spc
{

struct iDevice;

class SPC_CORE_API Projector
{
public:
  Projector();
  void UpdatePerspective(float angle, float aspect, float near, float far);
  void UpdatePerspective(float left, float right, float bottom, float top, float near, float far);
  void UpdateOrtho(float left, float right, float bottom, float top, float near, float far);

  void GetPoints(float depth, Vector3f* out) const;
  SPC_NODISCARD float GetNear() const;
  SPC_NODISCARD float GetFar() const;

  void Bind(iDevice* device) const;

private:

  eProjectionMode m_mode;

  float m_left;
  float m_right;
  float m_bottom;
  float m_top;
  float m_near;
  float m_far;

};

}