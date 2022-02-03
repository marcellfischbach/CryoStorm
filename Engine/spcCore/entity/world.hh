
#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>


namespace spc
{

class Entity;
struct iGfxScene;
struct iPhysicsWorld;
class EntityState;
class SpatialState;

SPC_CLASS()
class SPC_CORE_API World : public SPC_SUPER(iObject)
{
  SPC_CLASS_GEN_OBJECT;
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