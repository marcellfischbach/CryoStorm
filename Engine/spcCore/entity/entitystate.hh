
#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>

namespace spc
{

class Entity;
class World;

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

  World* GetWorld();
  const World* GetWorld() const;


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
  
};

}