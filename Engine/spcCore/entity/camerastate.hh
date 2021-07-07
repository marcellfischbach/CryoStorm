
#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/entity/spatialstate.hh>

#include <spcCore/graphics/camera.hh>
#include <spcCore/graphics/projector.hh>


namespace spc
{



SPC_CLASS()
class SPC_CORE_API CameraState : public SPC_SUPER(SpatialState)
{
  SPC_CLASS_GEN_OBJECT;
public:
  CameraState();
  ~CameraState () override = default;


  void SetNear(float near);
  float GetNear() const;

  void SetFar(float near);
  float GetFar() const;

  void SetAngle(float angle);
  float GetAngle() const;

  void SetAngleWidthHeight(float angleWidtHeight);
  float GetAngleWidthHeight() const;

  const Camera& GetCamera() const;
  const Projector& GetProjector() const;

  void Update(u32 width, u32 height);


private:
  SPC_PROPERTY()
    float m_near;
  SPC_PROPERTY()
    float m_far;

  SPC_PROPERTY()
    float m_angle;

  SPC_PROPERTY()
    float m_angleWidthHeight;


  Camera m_camera;
  Projector m_projector;

};


}