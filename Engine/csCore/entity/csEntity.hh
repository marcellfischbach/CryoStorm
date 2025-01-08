
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/csTypes.hh>
#include <csCore/csRef.hh>
#include <string>
#include <vector>

namespace cs
{


class csWorld;
class csEntityState;
class csSpatialState;


CS_CLASS(jclass = "org.cryo.core.entity.Entity")
class CS_CORE_API csEntity : public CS_SUPER(iObject)
{
CS_CLASS_GEN_OBJECT;
public:
  csEntity(const std::string &name = std::string(""));
  virtual ~csEntity();

  CS_FUNCTION()
  void SetName(const std::string &name);
  CS_FUNCTION()
  const std::string &GetName() const;

  CS_FUNCTION()
  void SetWorld(cs::csWorld *world);
  CS_FUNCTION()
  cs::csWorld *GetWorld();
  const csWorld *GetWorld() const;


  CS_FUNCTION()
  bool AttachEntity(csRef<cs::csEntity> &entity, csRef<cs::csSpatialState> &parentState = csRef<cs::csSpatialState>::Null());
  CS_FUNCTION()
  bool DetachEntity(csRef<cs::csEntity> &entity);
  CS_FUNCTION()
  csRef<cs::csEntity> &GetParent();
  const csRef<csEntity> &GetParent() const;
  CS_FUNCTION()
  size_t GetNumberOfChildren() const;
  CS_FUNCTION()
  csRef<cs::csEntity> &GetChild(size_t idx);
  const csRef<csEntity> &GetChild(size_t idx) const;


  template<typename ES>
  csRef<ES> &GetState()
  {
    return static_cast<csRef<ES>&>(GetState(ES::GetStaticClass()));
  }
  template<typename ES>
  const csRef<ES> &GetState() const
  {
    return static_cast<const csRef<ES>&>(GetState(ES::GetStaticClass()));
  }

  csRef<csEntityState> &GetState(const csClass *cls);
  const csRef<csEntityState> &GetState(const csClass *cls) const;


  template<typename ES>
  std::vector<csRef<ES>> GetStates()
  {
    std::vector<csRef<csEntityState>> states = GetStates(ES::GetStaticClass());
    std::vector<csRef<csEntityState>>* statesPtr = &states;
    std::vector<csRef<ES>>* esPtr = reinterpret_cast<std::vector<csRef<ES>>*>(esPtr);
    return *esPtr;
  }


  // TODO: Reimplement this in another way
  //template<typename ES>
  //std::vector<const csRef<ES>> GetStates() const
  //{
  //  std::vector<const csRef<csEntityState>> states = GetStates(ES::GetStaticClass());
  //  std::vector<const csRef<csEntityState>>* statesPtr = &states;
  //  std::vector<const csRef<ES>>* esPtr = reinterpret_cast<std::vector<csRef<ES>>*>(esPtr);
  //  return *esPtr;
  //}

  std::vector<csRef<csEntityState>> GetStates(const csClass *cls);
  //std::vector<const csRef<csEntityState>> GetStates(const csClass *cls) const;

  CS_FUNCTION()
  bool AttachState(csRef<cs::csEntityState> entityState);
  CS_FUNCTION()
  bool DetachState(csRef<cs::csEntityState> entityState);

  CS_FUNCTION()
  void SetRoot(csRef<cs::csSpatialState> &rootState);
  CS_FUNCTION()
  csRef<cs::csSpatialState> &GetRoot();
  const csRef<csSpatialState> &GetRoot() const;

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
  void RegisterEntityState(csRef<csEntityState> &entityState);
  void DeregisterEntityState(csRef<csEntityState> &entityState);

private:
  csRef<csSpatialState> &GetAbsolutRoot();
  csRef<csSpatialState> &GetAbsolutParentRoot();
  std::string m_name;

  csRef<csSpatialState> m_rootState;
  std::vector<csRef<csEntityState>> m_states;
  std::vector<csRef<csEntityState>> m_updateStates;

  csRef<csEntity> m_parent;
  std::vector<csRef<csEntity>> m_children;

  csWorld *m_world;

};

}