
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/csRef.hh>
#include <csCore/loaders/scene/csSceneLoader.hh>
#include <csCryoFile/csCryoFile.hh>
#include <set>
#include <vector>

namespace cs
{
using namespace file;

class csEntityState;

CS_CLASS()
class CS_CORE_API csEntityStateLoaderBase : public CS_SUPER(iObject)
{
CS_CLASS_GEN_OBJECT;
public:
  ~csEntityStateLoaderBase() override = default;

  bool CanLoad(const csClass *cls) const;
  int Priority ();


  virtual csOwned<csEntityState> LoadState(const csCryoFile *file,
                                           const csCryoFileElement *stateElement,
                                           const csAssetLocator &locator,
                                           csSceneLoaderData *data) const = 0;

protected:
  explicit csEntityStateLoaderBase(int priority);

  void RegisterClass(const csClass *cls);


private:
  int m_priority;
  std::set<const csClass *> m_validClasses;

};


class CS_CORE_API csEntityStateLoaderRegistry
{
public:
  static csEntityStateLoaderRegistry &Get();

  csEntityStateLoaderRegistry &Register(csEntityStateLoaderBase *loader);

  const std::vector<csEntityStateLoaderBase *> &GetStateLoaders() const;


private:
  csEntityStateLoaderRegistry() = default;

  static csEntityStateLoaderRegistry *static_instance;

  std::vector<csEntityStateLoaderBase *> m_stateLoaders;
};

} // cs
