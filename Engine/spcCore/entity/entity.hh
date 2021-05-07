
#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>
#include <spcCore/types.hh>
#include <string>
#include <vector>

namespace spc
{

class World;
class EntityState;
class SpatialState;

SPC_CLASS()
class SPC_CORE_API Entity : public SPC_SUPER(iObject)
{

  SPC_CLASS_GEN_OBJECT;
public:
  Entity(const std::string &name = std::string(""));
  virtual ~Entity();
  
  void SetName(const std::string &name);
  const std::string &GetName() const;

  void SetWorld(World * world);
  World* GetWorld();
  const World* GetWorld() const;

  bool Attach(Entity * entity, SpatialState *parentState = nullptr);
  bool Detach(Entity * entity);
  Entity* GetParent();
  const Entity* GetParent() const;
  Size GetNumberOfChildren() const;
  Entity* GetChild(Size idx);
  const Entity* GetChild(Size idx) const;
  



  bool Attach(EntityState* entityState);
  bool Detach(EntityState* entityState);
  
  void SetRoot(SpatialState *rootState);
  SpatialState*GetRoot();
  const SpatialState*GetRoot() const;

  /**
   * @name Lifecycle
   * @{
   */

  /**
   * @}
   */

  
  /**
   * Don't use directly will be called by the entity states
   * @param entityState
   */
  void RegisterEntityState(EntityState* entityState);
  void DeregisterEntityState(EntityState* entityState);
  
private:
  SpatialState *GetAbsolutRoot();
  SpatialState *GetAbsolutParentRoot();
  std::string m_name;
  
  SpatialState* m_rootState;
  std::vector<EntityState*> m_states;
  std::vector<EntityState*> m_updateStates;

  Entity* m_parent;
  std::vector<Entity*> m_children;
  
  World* m_world;
  
};

}