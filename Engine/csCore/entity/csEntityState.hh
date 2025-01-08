
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

CS_CLASS(jclass="org.cryo.core.entity.EntityState")
class CS_CORE_API csEntityState : public CS_SUPER(iObject)
{
CS_CLASS_GEN_OBJECT;
public:
  explicit csEntityState(const std::string& name = std::string(""));
  virtual ~csEntityState() = default;

  CS_FUNCTION()
  void SetName(const std::string& name);
  CS_FUNCTION()
  const std::string& GetName() const;

  CS_FUNCTION()
  void SetEntity(csRef<cs::csEntity> &entity);

  CS_FUNCTION()
  csRef<cs::csEntity> &GetEntity();
  const csRef<csEntity> &GetEntity() const;

  template<typename ES>
  csRef<ES>& GetState()
  {
    return static_cast<csRef<ES>&>(GetState(ES::GetStaticClass()));
  }
  template<typename ES>
  const csRef<ES>& GetState() const
  {
    return static_cast<const csRef<ES>&>(GetState(ES::GetStaticClass()));
  }

  csRef<csEntityState>& GetState(const csClass * cls);
  const csRef<csEntityState>& GetState(const csClass * cls) const;


  template<typename ES>
  std::vector<csRef<ES>> GetStates()
  {
    std::vector<csRef<csEntityState>> states = GetStates(ES::GetStaticClass());
    std::vector<csRef<csEntityState>>* statesPtr = &states;
    std::vector<csRef<ES>>* esPtr = reinterpret_cast<std::vector<csRef<ES>>*>(statesPtr);
    return *esPtr;
  }

  //template<typename ES>
  //std::vector<const csRef<ES>> GetStates() const
  //{
  //  std::vector<const csRef<csEntityState>> states = GetStates(ES::GetStaticClass());
  //  std::vector<const csRef<csEntityState>>* statesPtr = &states;
  //  std::vector<const csRef<ES>>* esPtr = reinterpret_cast<std::vector<csRef<ES>>*>(esPtr);
  //  return *esPtr;
  //}

  std::vector<csRef<csEntityState>> GetStates(const csClass* cls);
  //std::vector<const csRef<csEntityState>> GetStates(const csClass* cls) const;

  CS_FUNCTION()
  csRef<cs::csSpatialState>& GetRoot();
  const csRef<csSpatialState>& GetRoot() const;

  CS_FUNCTION()
  cs::csWorld* GetWorld();
  const csWorld* GetWorld() const;

  CS_FUNCTION()
  void AttachToWorld(cs::csWorld* world);
  CS_FUNCTION()
  void DetachFromWorld(cs::csWorld* world);

  CS_FUNCTION()
  void SetNeedUpdate(bool needUpdate);
  CS_FUNCTION()
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
  virtual void UpdateEntity(csRef<csEntity> &oldEntity, csRef<csEntity> &newEntity);

private:
  std::string m_name;
  csRef<csEntity> m_entity;

  bool m_needUpdate;

};

}