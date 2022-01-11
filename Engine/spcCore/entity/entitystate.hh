
#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>

namespace spc
{

class Entity;
class World;
class SpatialState;

SPC_CLASS()
class SPC_CORE_API EntityState : public SPC_SUPER(iObject)
{
SPC_CLASS_GEN_OBJECT;
public:
  EntityState(const std::string &name = std::string(""));
  virtual ~EntityState();
  
  void SetName(const std::string &name);
  const std::string &GetName() const;
  
  
  void SetEntity(Entity* entity);
  Entity* GetEntity();
  const Entity* GetEntity() const;


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