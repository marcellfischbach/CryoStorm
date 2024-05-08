
#pragma once

#include <ceCore/entity/entitysystem.hh>

CE_CLASS()
class MirrorHandler : public CE_SUPER(ce::EntityState)
{
  CE_CLASS_GEN_OBJECT;
public:
  MirrorHandler();
  ~MirrorHandler() override;

  void SetCameraState(ce::SpatialState *camera);

  void Update (float tpf) override;

private:
  ce::SpatialState *m_cameraState;
};