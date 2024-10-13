
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <array>
#include <thread>

namespace cs
{

class csEntity;
struct iGfxScene;
struct iPhysicsWorld;
class csEntityState;
class csSpatialState;
class csCameraState;


CS_CLASS(jclass="org.cryo.core.entity.World")
class CS_CORE_API csWorld : public CS_SUPER(iObject)
{
  CS_CLASS_GEN_OBJECT;
public:
  csWorld();

  CS_FUNCTION()
  void SetScene(cs::iGfxScene * scene);
  CS_FUNCTION()
  CS_NODISCARD cs::iGfxScene* GetScene();
  CS_NODISCARD const iGfxScene* GetScene() const;

  CS_FUNCTION()
  void SetPhysicsWorld(cs::iPhysicsWorld *world);
  CS_FUNCTION()
  CS_NODISCARD cs::iPhysicsWorld *GetPhysicsWorld();
  CS_NODISCARD const iPhysicsWorld *GetPhysicsWorld() const;

  CS_FUNCTION()
  void SetMainCamera(cs::csCameraState *mainCamera);
  CS_FUNCTION()
  CS_NODISCARD cs::csCameraState *GetMainCamera();
  CS_NODISCARD const csCameraState *GetMainCamera() const;

  CS_FUNCTION()
  bool Attach(cs::csEntity * entity);
  CS_FUNCTION()
  bool Detach(cs::csEntity * entity);

  CS_FUNCTION()
  bool AttachUpdateState(cs::csEntityState *updateState);
  CS_FUNCTION()
  bool DetachUpdateState(cs::csEntityState *updateState);

  CS_FUNCTION()
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
