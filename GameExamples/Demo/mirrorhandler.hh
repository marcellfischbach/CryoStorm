
#pragma once

#include <csCore/entity/csEntitySystem.hh>

CS_CLASS()
class MirrorHandler : public CS_SUPER(cs::csEntityState)
{
  CS_CLASS_GEN_OBJECT;
public:
  MirrorHandler();
  ~MirrorHandler() override;

  void SetCameraState(cs::csSpatialState *camera);

  void Update (float tpf) override;

private:
  cs::csSpatialState *m_cameraState;
};