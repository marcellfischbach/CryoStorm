
#pragma once

#include <ceCore/entity/entitysystem.hh>

CS_CLASS()
class MirrorHandler : public CS_SUPER(cryo::EntityState)
{
  CS_CLASS_GEN_OBJECT;
public:
  MirrorHandler();
  ~MirrorHandler() override;

  void SetCameraState(cryo::SpatialState *camera);

  void Update (float tpf) override;

private:
  cryo::SpatialState *m_cameraState;
};