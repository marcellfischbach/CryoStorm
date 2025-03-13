
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/resource/csBaseCSFAssetLoader.hh>


namespace cs
{

using namespace file;


class csEntity;

struct csSceneLoaderData
{
  std::map<std::string, csEntity *> namedEntities;
};


CS_CLASS()
class CS_CORE_API csSceneLoader : public CS_SUPER(csBaseCSFAssetLoader)
{
public:
CS_CLASS_GEN;

  csSceneLoader();
  ~csSceneLoader() override = default;


protected:
  csOwned<iAsset> Load(const csCryoFile *file, const csAssetLocator &locator) const override;


private:
  csOwned<csEntity> LoadEntity(const csCryoFile *file,
                               const csCryoFileElement *entityElement,
                               const csAssetLocator &locator,
                               csSceneLoaderData *data) const;


  void LoadStates(const csCryoFile *file,
                  const csCryoFileElement *statesElement,
                  const csAssetLocator &locator,
                  csEntity *entity,
                  csSceneLoaderData *data) const;

  void LoadState(const csCryoFile *file,
                 const csCryoFileElement *stateElement,
                 const csAssetLocator &locator,
                 csEntity *entity,
                 csSceneLoaderData *data) const;

  void LoadChildren(const csCryoFile *file,
                    const csCryoFileElement *childrenElement,
                    const csAssetLocator &locator,
                    csEntity *entity,
                    csSceneLoaderData *data) const;


};

} // cs
