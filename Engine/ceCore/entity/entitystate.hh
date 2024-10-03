
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <vector>

namespace ce
{

class Entity;
class World;
class SpatialState;

CS_CLASS()
class CS_CORE_API EntityState : public CS_SUPER(iObject)
{
CS_CLASS_GEN_OBJECT;
public:
  EntityState(const std::string& name = std::string(""));
  virtual ~EntityState();

  void SetName(const std::string& name);
  const std::string& GetName() const;

  void SetEntity(Entity* entity);
  Entity* GetEntity();
  const Entity* GetEntity() const;

  template<typename ES>
  ES* GetState()
  {
    return static_cast<ES*>(GetState(ES::GetStaticClass()));
  }
  template<typename ES>
  const ES* GetState() const
  {
    return static_cast<const ES*>(GetState(ES::GetStaticClass()));
  }

  EntityState* GetState(const Class* cls);
  const EntityState* GetState(const Class* cls) const;

  template<typename ES>
  std::vector<ES*> GetStates()
  {
    std::vector<EntityState*> states = GetStates(ES::GetStaticClass());
    return *reinterpret_cast<std::vector<ES*>*>(&states);
  }

  template<typename ES>
  std::vector<const ES*> GetStates() const
  {
    return *reinterpret_cast<std::vector<const ES*>*>(&GetStates(ES::GetStaticClass()));
  }

  std::vector<EntityState*> GetStates(const Class* cls);
  std::vector<const EntityState*> GetStates(const Class* cls) const;

  SpatialState* GetRoot();
  const SpatialState* GetRoot() const;

  World* GetWorld();
  const World* GetWorld() const;

  void AttachToWorld(World* world);
  void DetachFromWorld(World* world);

  void SetNeedUpdate(bool needUpdate);
  bool IsNeedUpdate() const;

  /**
   * @name Livecycle
   * @{
   */

  virtual void OnAttachedToWorld(World* world);
  virtual void OnDetachedFromWorld(World* world);

  virtual void Update(float tpf);
  /**
   * @}
   */

protected:
  virtual void UpdateEntity(Entity* oldEntity, Entity* newEntity);

private:
  std::string m_name;
  Entity* m_entity;

  bool m_needUpdate;

};

}