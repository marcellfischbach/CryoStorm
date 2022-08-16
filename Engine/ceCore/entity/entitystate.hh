
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>

namespace ce
{

class Entity;
class World;
class SpatialState;

CE_CLASS()
class CE_CORE_API EntityState : public CE_SUPER(iObject)
{
CE_CLASS_GEN_OBJECT;
public:
  EntityState(const std::string &name = std::string(""));
  virtual ~EntityState();
  
  void SetName(const std::string &name);
  const std::string &GetName() const;
  
  
  void SetEntity(Entity* entity);
  Entity* GetEntity();
  const Entity* GetEntity() const;

  template <typename ES>
  ES* GetState()
  {
    return static_cast<ES*>(GetState(ES::StaticClass()));
  }
  template <typename ES>
  const ES* GetState() const
  {
    return static_cast<const ES*>(GetState(ES::StaticClass()));
  }

  EntityState* GetState(const Class * cls);
  const EntityState* GetState(const Class * cls) const;

  SpatialState* GetRoot();
  const SpatialState* GetRoot() const;

  World* GetWorld();
  const World* GetWorld() const;

  void AttachToWorld(World *world);
  void DetachFromWorld(World *world);


  void SetNeedUpdate(bool needUpdate);
  bool IsNeedUpdate() const;

  /**
   * @name Livecycle
   * @{
   */

  virtual void OnAttachedToWorld(World * world);
  virtual void OnDetachedFromWorld(World * world);

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