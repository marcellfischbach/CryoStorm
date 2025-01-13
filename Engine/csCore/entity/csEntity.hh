
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
  bool AttachEntity(cs::csEntity *entity, cs::csSpatialState *parentState = nullptr);
  CS_FUNCTION()
  bool DetachEntity(cs::csEntity *entity);
  CS_FUNCTION()
  cs::csEntity *GetParent();
  const csEntity *GetParent() const;
  CS_FUNCTION()
  size_t GetNumberOfChildren() const;
  CS_FUNCTION()
  cs::csEntity *GetChild(size_t idx);
  const csEntity *GetChild(size_t idx) const;


  template<typename ES>
  ES *GetState()
  {
    return static_cast<ES *>(GetState(ES::GetStaticClass()));
  }
  template<typename ES>
  const ES *GetState() const
  {
    return static_cast<const ES *>(GetState(ES::GetStaticClass()));
  }

  csEntityState *GetState(const csClass *cls);
  const csEntityState *GetState(const csClass *cls) const;


  template<typename ES>
  std::vector<ES *> GetStates()
  {
    return *reinterpret_cast<std::vector<ES *> *>(&GetStates(ES::GetStaticClass()));
  }

  template<typename ES>
  std::vector<const ES *> GetStates() const
  {
    return *reinterpret_cast<std::vector<const ES *> *>(&GetStates(ES::GetStaticClass()));
  }

  std::vector<csEntityState *> GetStates(const csClass *cls);
  std::vector<const csEntityState *> GetStates(const csClass *cls) const;

  CS_FUNCTION()
  bool AttachState(cs::csEntityState *entityState);
  CS_FUNCTION()
  bool DetachState(cs::csEntityState *entityState);

  CS_FUNCTION()
  void SetRoot(cs::csSpatialState *rootState);
  CS_FUNCTION()
  cs::csSpatialState *GetRoot();
  const csSpatialState *GetRoot() const;

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
  void RegisterEntityState(csEntityState *entityState);
  void DeregisterEntityState(csEntityState *entityState);

private:
  csSpatialState *GetAbsolutRoot();
  csSpatialState *GetAbsolutParentRoot();
  std::string m_name;

  csRef<csSpatialState> m_rootState;
  std::vector<csRef<csEntityState>> m_states;
  std::vector<csRef<csEntityState>> m_updateStates;

  csRef<csEntity> m_parent;
  std::vector<csRef<csEntity>> m_children;

  csWorld *m_world;

};

}