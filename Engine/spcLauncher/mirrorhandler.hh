
#pragma once

#include <spcCore/entity/entitysystem.hh>

SPC_CLASS()
class MirrorHandler : public SPC_SUPER(spc::EntityState)
{
  SPC_CLASS_GEN_OBJECT;
public:
  MirrorHandler();
  ~MirrorHandler() override;

  void SetCameraState(spc::SpatialState *camera);

  void Update (float tpf) override;

private:
  spc::SpatialState *m_cameraState;
};