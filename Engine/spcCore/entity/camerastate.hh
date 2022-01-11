
#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/entity/spatialstate.hh>

#include <spcCore/graphics/camera.hh>
#include <spcCore/graphics/projector.hh>
#include <spcCore/graphics/eclearmode.hh>
#include <spcCore/math/color4f.hh>

namespace spc
{

class GfxCamera;


SPC_CLASS()
class SPC_CORE_API CameraState : public SPC_SUPER(SpatialState)
{
  SPC_CLASS_GEN_OBJECT;
public:
  CameraState();
  ~CameraState () override;


  void SetNear(float near);
  float GetNear() const;

  void SetFar(float near);
  float GetFar() const;

  void SetAngle(float angle);
  float GetAngle() const;

  void SetAngleWidthHeight(float angleWidtHeight);
  float GetAngleWidthHeight() const;

  void SetClearMode (eClearMode clearMode);
  eClearMode GetClearMode () const;

  void SetClearColor(const Color4f &clearColor);
  const Color4f &GetClearColor() const;

  void SetClearDepth(float clearDepth);
  float GetClearDepth () const;

  const Camera& GetCamera() const;
  const Projector& GetProjector() const;

  void Update(u32 width, u32 height);

  void OnAttachedToWorld(World *world) override;
  void OnDetachedFromWorld(World *world) override;

protected:
  void TransformationUpdatedPreChildren() override;

private:
  SPC_PROPERTY()
    float m_near;
  SPC_PROPERTY()
    float m_far;

  SPC_PROPERTY()
    float m_angle;

  SPC_PROPERTY()
    float m_angleWidthHeight;

  SPC_PROPERTY()
    eClearMode m_clearMode = eClearMode::DepthColor;

  SPC_PROPERTY()
    Color4f m_clearColor = Color4f();

  SPC_PROPERTY()
    float m_clearDepth = 1.0f;


  Camera m_camera;
  Projector m_projector;

  void UpdateGfxCamera ();
  GfxCamera *m_gfxCamera;


};


}