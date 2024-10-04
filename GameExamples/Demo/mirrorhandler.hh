
#pragma once

#include <csCore/entity/csEntitySystem.hh>

CS_CLASS()
class MirrorHandler : public CS_SUPER(cryo::csEntityState)
{
  CS_CLASS_GEN_OBJECT;
public:
  MirrorHandler();
  ~MirrorHandler() override;

  void SetCameraState(cryo::csSpatialState *camera);

  void Update (float tpf) override;

private:
  cryo::csSpatialState *m_cameraState;
};