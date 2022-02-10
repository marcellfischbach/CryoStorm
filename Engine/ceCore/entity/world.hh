
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>


namespace ce
{

class Entity;
struct iGfxScene;
struct iPhysicsWorld;
class EntityState;
class SpatialState;

CE_CLASS()
class CE_CORE_API World : public CE_SUPER(iObject)
{
  CE_CLASS_GEN_OBJECT;
public:
  World();
  
  void SetScene(iGfxScene * scene);
  iGfxScene* GetScene();
  const iGfxScene* GetScene() const;

  void SetPhysicsWorld(iPhysicsWorld *world);
  iPhysicsWorld *GetPhysicsWorld();
  const iPhysicsWorld *GetPhysicsWorld() const;

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
};

}