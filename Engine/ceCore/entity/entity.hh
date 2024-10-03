
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/types.hh>
#include <string>
#include <vector>

namespace cryo
{


class World;
class EntityState;
class SpatialState;

CS_CLASS(jclass="org.crimsonedge.core.entity.Entity")
class CS_CORE_API Entity : public CS_SUPER(iObject)
{
  CS_CLASS_GEN_OBJECT;
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
  

  template <typename ES>
  ES* GetState()
  {
    return static_cast<ES*>(GetState(ES::GetStaticClass()));
  }
  template <typename ES>
  const ES* GetState() const
  {
    return static_cast<const ES*>(GetState(ES::GetStaticClass()));
  }

  EntityState* GetState(const Class * cls);
  const EntityState* GetState(const Class * cls) const;


  template<typename ES>
  std::vector<ES*> GetStates()
  {
    return *reinterpret_cast<std::vector<ES*>*>(&GetStates(ES::GetStaticClass()));
  }

  template<typename ES>
  std::vector<const ES*> GetStates() const
  {
    return *reinterpret_cast<std::vector<const ES*>*>(&GetStates(ES::GetStaticClass()));
  }

  std::vector<EntityState*> GetStates(const Class * cls);
  std::vector<const EntityState*> GetStates(const Class * cls) const;

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