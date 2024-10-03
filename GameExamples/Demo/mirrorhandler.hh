
#pragma once

#include <ceCore/entity/entitysystem.hh>

CS_CLASS()
class MirrorHandler : public CS_SUPER(ce::EntityState)
{
  CS_CLASS_GEN_OBJECT;
public:
  MirrorHandler();
  ~MirrorHandler() override;

  void SetCameraState(ce::SpatialState *camera);

  void Update (float tpf) override;

private:
  ce::SpatialState *m_cameraState;
};