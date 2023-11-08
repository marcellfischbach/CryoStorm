

#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/graphics/eprojectionmode.hh>
#include <ceCore/math/matrix4f.hh>
#include <ceCore/math/vector3f.hh>

namespace ce
{

struct iDevice;

CE_CLASS()
class CE_CORE_API Projector : public CE_SUPER(iObject)
{
CE_CLASS_GEN_OBJECT;
public:
  Projector();
  ~Projector() override = default;

  void UpdatePerspective(float angle, float aspect, float near, float far);
  void UpdatePerspective(float left, float right, float bottom, float top, float near, float far);
  void UpdateOrtho(float left, float right, float bottom, float top, float near, float far);

  void GetPoints(float depth, Vector3f* out) const;
  CE_NODISCARD float GetLeft () const;
  CE_NODISCARD float GetRight () const;
  CE_NODISCARD float GetBottom () const;
  CE_NODISCARD float GetTop () const;
  CE_NODISCARD float GetNear() const;
  CE_NODISCARD float GetFar() const;

  void Bind(iDevice* device) const;
  Matrix4f GetProjectionMatrix(iDevice *device) const;
  Matrix4f GetProjectionMatrixInv(iDevice *device) const;


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