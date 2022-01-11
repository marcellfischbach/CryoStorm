#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>
#include <spcCore/graphics/eclearmode.hh>
#include <spcCore/math/color4f.hh>

namespace spc
{

class Camera;
class Projector;
struct iRenderTarget;

SPC_CLASS()
class GfxCamera : public SPC_SUPER(iObject)
{
  SPC_CLASS_GEN_OBJECT;
public:
  GfxCamera();
  ~GfxCamera() override;

  void SetCamera(Camera *camera);
  const Camera* GetCamera() const;
  Camera* GetCamera();

  void SetProjector(Projector *projector);
  const Projector* GetProjector() const;
  Projector* GetProjector();

  eClearMode GetClearMode () const;
  const Color4f &GetClearColor () const;
  float GetClearDepth() const;

  void SetRenderTarget (iRenderTarget* renderTarget);
  iRenderTarget* GetRenderTarget();
  const iRenderTarget* GetRenderTarget() const;

  void SetPriority (int priority);
  int GetPriority() const;

  void UpdateData (float near, float far, float angle, float angleWidthHeight);

  void UpdateClear (eClearMode mode, const Color4f &color, float depth);

  void UpdateProjector (uint32_t width, uint32_t height);

private:

  float m_near;
  float m_far;
  float m_angle;
  float m_angleWidthHeight;

  eClearMode m_clearMode;
  Color4f m_clearColor;
  float m_clearDepth;

  Camera* m_camera;
  Projector *m_projector;

  iRenderTarget *m_renderTarget;
  int m_priority;

};


}

