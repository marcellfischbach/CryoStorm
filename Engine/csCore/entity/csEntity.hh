
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/csTypes.hh>
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

  void SetWorld(csWorld *world);
  csWorld *GetWorld();
  const csWorld *GetWorld() const;

  bool Attach(cs::csEntity *entity, cs::csSpatialState *parentState = nullptr);
  bool Detach(csEntity *entity);
  csEntity *GetParent();
  const csEntity *GetParent() const;
  Size GetNumberOfChildren() const;
  csEntity *GetChild(Size idx);
  const csEntity *GetChild(Size idx) const;


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
  bool Attach(cs::csEntityState *entityState);
  CS_FUNCTION()
  bool Detach(cs::csEntityState *entityState);

  void SetRoot(csSpatialState *rootState);
  csSpatialState *GetRoot();
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

  csSpatialState *m_rootState;
  std::vector<csEntityState *> m_states;
  std::vector<csEntityState *> m_updateStates;

  csEntity *m_parent;
  std::vector<csEntity *> m_children;

  csWorld *m_world;

};

}