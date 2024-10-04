
#pragma once

#include <ceCore/csCoreExport.hh>
#include <ceCore/csClass.hh>
#include <array>
#include <thread>

namespace cryo
{

class csEntity;
struct iGfxScene;
struct iPhysicsWorld;
class csEntityState;
class csSpatialState;
class csCameraState;

CS_CLASS(jclass="org.crimsonedge.core.entity.World")
class CS_CORE_API csWorld : public CS_SUPER(iObject)
{
  CS_CLASS_GEN_OBJECT;
public:
  csWorld();
  
  void SetScene(iGfxScene * scene);
  CS_NODISCARD iGfxScene* GetScene();
  CS_NODISCARD const iGfxScene* GetScene() const;

  void SetPhysicsWorld(iPhysicsWorld *world);
  CS_NODISCARD iPhysicsWorld *GetPhysicsWorld();
  CS_NODISCARD const iPhysicsWorld *GetPhysicsWorld() const;

  void SetMainCamera(csCameraState *mainCamera);
  CS_NODISCARD csCameraState *GetMainCamera();
  CS_NODISCARD const csCameraState *GetMainCamera() const;

  bool Attach(csEntity * entity);
  bool Detach(csEntity * entity);

  bool AttachUpdateState(csEntityState *updateState);
  bool DetachUpdateState(csEntityState *updateState);

  void Update (float tpf);

private:
  iGfxScene * m_scene;
  iPhysicsWorld *m_physicsWorld;
  float m_physicsUpdateCounter;

  csSpatialState *m_rootState;
  
  std::vector<csEntity*>      m_entities;
  std::vector<csEntityState*> m_updateStates;

  csCameraState *m_mainCamera = nullptr;

  std::array<std::thread, 4> m_threads;
};

}
