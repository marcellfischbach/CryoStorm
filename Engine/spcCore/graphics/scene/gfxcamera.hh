#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>

namespace spc
{

class Camera;
class Projector;

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

  void UpdateData (float near, float far, float angle, float angleWidthHeight);

  void UpdateProjector (uint32_t width, uint32_t height);

private:

  float m_near;
  float m_far;
  float m_angle;
  float m_angleWidthHeight;


  Camera* m_camera;
  Projector *m_projector;


};


}

