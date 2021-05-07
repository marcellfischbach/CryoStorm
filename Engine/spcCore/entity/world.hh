
#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>

namespace spc
{

class Entity;
class GfxScene;
class EntityState;
class SpatialState;

SPC_CLASS()
class SPC_CORE_API World : public SPC_SUPER(iObject)
{
  SPC_CLASS_GEN_OBJECT;
public:
  World();
  
  void SetScene(GfxScene * scene);
  GfxScene* GetScene();
  const GfxScene* GetScene() const;

  bool Attach(Entity * entity);
  bool Detach(Entity * entity);

  bool AttachUpdateState(EntityState *updateState);
  bool DetachUpdateState(EntityState *updateState);

  void Update (float tpf);

private:
  GfxScene* m_scene;
  SpatialState *m_rootState;
  
  std::vector<Entity*> m_entities;

  std::vector<EntityState*> m_updateStates;
};

}