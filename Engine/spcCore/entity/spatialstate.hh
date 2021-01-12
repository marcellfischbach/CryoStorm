
#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/entity/entitystate.hh>
#include <vector>

namespace spc
{


SPC_CLASS()
class SPC_CORE_API SpatialState : public SPC_SUPER(EntityState)
{
  SPC_CLASS_GEN;
public:
  SpatialState(const std::string &name = std::string(""));
  virtual ~SpatialState();
  
  bool Attach(SpatialState *child);
  bool Detach(SpatialState *child);
  SpatialState* GetParent();
  const SpatialState* GetParent() const;
  
protected:
  void UpdateEntity(Entity* oldEntity, Entity* newEntity) override;
  
private:
  SpatialState* m_parent;
  std::vector<SpatialState*> m_children;
};

}