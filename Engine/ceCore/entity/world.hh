
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <array>
#include <thread>

namespace ce
{

class Entity;
struct iGfxScene;
struct iPhysicsWorld;
class EntityState;
class SpatialState;
class CameraState;

CS_CLASS(jclass="org.crimsonedge.core.entity.World")
class CS_CORE_API World : public CS_SUPER(iObject)
{
  CS_CLASS_GEN_OBJECT;
public:
  World();
  
  void SetScene(iGfxScene * scene);
  CS_NODISCARD iGfxScene* GetScene();
  CS_NODISCARD const iGfxScene* GetScene() const;

  void SetPhysicsWorld(iPhysicsWorld *world);
  CS_NODISCARD iPhysicsWorld *GetPhysicsWorld();
  CS_NODISCARD const iPhysicsWorld *GetPhysicsWorld() const;

  void SetMainCamera(CameraState *mainCamera);
  CS_NODISCARD CameraState *GetMainCamera ();
  CS_NODISCARD const CameraState *GetMainCamera () const;

  bool Attach(Entity * entity);
  bool Detach(Entity * entity);

  bool AttachUpdateState(EntityState *updateState);
  bool DetachUpdateState(EntityState *updateState);

  void Update (float tpf);

private:
  iGfxScene * m_scene;
  iPhysicsWorld *m_physicsWorld;
  float m_physicsUpdateCounter;

  SpatialState *m_rootState;
  
  std::vector<Entity*> m_entities;
  std::vector<EntityState*> m_updateStates;

  CameraState *m_mainCamera = nullptr;

  std::array<std::thread, 4> m_threads;
};

}
