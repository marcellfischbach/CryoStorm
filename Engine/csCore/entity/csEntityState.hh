
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/csRef.hh>
#include <vector>

namespace cs
{

class csEntity;
class csWorld;
class csSpatialState;

CS_CLASS(jclass = "org.cryo.core.entity.EntityState")
class CS_CORE_API csEntityState : public CS_SUPER(iObject)
{
CS_CLASS_GEN_OBJECT;
public:
  explicit csEntityState(const std::string &name = std::string(""));
  virtual ~csEntityState() = default;

  CS_FUNCTION()
  void SetName(const std::string &name);
  CS_FUNCTION()
  const std::string &GetName() const;

  CS_FUNCTION()
  void SetEntity(cs::csEntity*entity);

  CS_FUNCTION()
  cs::csEntity*GetEntity();
  const csEntity *GetEntity() const;

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

  csEntityState* GetState(const csClass* cls);
  const csEntityState* GetState(const csClass* cls) const;

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

  std::vector<csEntityState*> GetStates(const csClass* cls);
  std::vector<const csEntityState*> GetStates(const csClass* cls) const;

  CS_FUNCTION()
  cs::csSpatialState *GetRoot();
  const csSpatialState *GetRoot() const;

  CS_FUNCTION()
  cs::csWorld *GetWorld();
  const csWorld *GetWorld() const;

  CS_FUNCTION()
  void AttachToWorld(cs::csWorld *world);
  CS_FUNCTION()
  void DetachFromWorld(cs::csWorld *world);

  CS_FUNCTION()
  void SetNeedUpdate(bool needUpdate);
  CS_FUNCTION()
  bool IsNeedUpdate() const;

  /**
   * @name Livecycle
   * @{
   */

  virtual void OnAttachedToWorld(csWorld *world);
  virtual void OnDetachedFromWorld(csWorld *world);

  virtual void Update(float tpf);
  /**
   * @}
   */

protected:
  virtual void UpdateEntity(csEntity *oldEntity, csEntity *newEntity);

private:
  std::string     m_name;
  csRef<csEntity> m_entity;

  bool m_needUpdate;

};

}