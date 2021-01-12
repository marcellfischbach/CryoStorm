
#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>
#include <string>
#include <vector>

namespace spc
{

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
  
  bool Attach(EntityState* entityState);
  bool Detach(EntityState* entityState);
  
  void SetRoot(SpatialState *rootState);
  SpatialState*GetRoot();
  const SpatialState*GetRoot() const;
  
  /**
   * Don't use directly will be called by the entity states
   * @param entityState
   */
  void RegisterEntityState(EntityState* entityState);
  void DeregisterEntityState(EntityState* entityState);
  
private:
  std::string m_name;
  
  SpatialState* m_rootState;
  std::vector<EntityState*> m_states;
  
  
};

}