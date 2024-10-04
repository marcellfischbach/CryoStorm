
#pragma once

#include <ceCore/csCoreExport.hh>
#include <ceCore/class.hh>
#include <vector>

namespace cryo
{

class csEntity;
class csWorld;
class csSpatialState;

CS_CLASS()
class CS_CORE_API csEntityState : public CS_SUPER(iObject)
{
CS_CLASS_GEN_OBJECT;
public:
  csEntityState(const std::string& name = std::string(""));
  virtual ~csEntityState();

  void SetName(const std::string& name);
  const std::string& GetName() const;

  void SetEntity(csEntity* entity);
  csEntity* GetEntity();
  const csEntity* GetEntity() const;

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

  csEntityState* GetState(const Class* cls);
  const csEntityState* GetState(const Class* cls) const;

  template<typename ES>
  std::vector<ES*> GetStates()
  {
    std::vector<csEntityState*> states = GetStates(ES::GetStaticClass());
    return *reinterpret_cast<std::vector<ES*>*>(&states);
  }

  template<typename ES>
  std::vector<const ES*> GetStates() const
  {
    return *reinterpret_cast<std::vector<const ES*>*>(&GetStates(ES::GetStaticClass()));
  }

  std::vector<csEntityState*> GetStates(const Class* cls);
  std::vector<const csEntityState*> GetStates(const Class* cls) const;

  csSpatialState* GetRoot();
  const csSpatialState* GetRoot() const;

  csWorld* GetWorld();
  const csWorld* GetWorld() const;

  void AttachToWorld(csWorld* world);
  void DetachFromWorld(csWorld* world);

  void SetNeedUpdate(bool needUpdate);
  bool IsNeedUpdate() const;

  /**
   * @name Livecycle
   * @{
   */

  virtual void OnAttachedToWorld(csWorld* world);
  virtual void OnDetachedFromWorld(csWorld* world);

  virtual void Update(float tpf);
  /**
   * @}
   */

protected:
  virtual void UpdateEntity(csEntity* oldEntity, csEntity* newEntity);

private:
  std::string m_name;
  csEntity * m_entity;

  bool m_needUpdate;

};

}