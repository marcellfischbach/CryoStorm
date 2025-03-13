//
// Created by Marcell on 13.03.2025.
//

#include <csCore/loaders/scene/csSceneLoader.hh>
#include <csCore/loaders/scene/csEntityStateLoaderBase.hh>
#include <csCore/entity/csEntity.hh>
#include <csCore/entity/csEntityState.hh>
#include <csCore/csClassRegistry.hh>
#include <map>

namespace cs
{


csSceneLoader::csSceneLoader()
{
  RegisterType("ENTITY");
}

csOwned<iAsset> csSceneLoader::Load(const file::csCryoFile *file, const csAssetLocator &locator) const
{

  if (locator.GetExtension() == "ENTITY")
  {
    const csCryoFileElement *rootElement   = file->Root();
    const csCryoFileElement *entityElement = rootElement->GetChild("entity");
    if (entityElement)
    {
      csSceneLoaderData data;
      return LoadEntity(file, entityElement, locator, &data);
    }
  }
  return nullptr;
}

csOwned<csEntity> csSceneLoader::LoadEntity(const csCryoFile *file,
                                            const csCryoFileElement *entityElement,
                                            const csAssetLocator &locator,
                                            csSceneLoaderData *data) const
{
  if (entityElement->GetTagName() != "entity")
  {
    return csOwned<csEntity>();
  }


  csEntity *entity = new csEntity();

  if (entityElement->HasAttribute("id"))
  {
    data->namedEntities[entityElement->GetAttribute("id", "")] = entity;
  }

  LoadStates(file, entityElement->GetChild("states"), locator, entity, data);
  LoadChildren(file, entityElement->GetChild("children"), locator, entity, data);

  return csOwned<csEntity>(entity);
}

void csSceneLoader::LoadStates(const csCryoFile *file,
                               const csCryoFileElement *statesElement,
                               const csAssetLocator &locator,
                               csEntity *entity,
                               csSceneLoaderData *data) const
{

  for (int i = 0; i < statesElement->GetNumberOfChildren(); ++i)
  {
    const csCryoFileElement *childElement = statesElement->GetChild(i);
    if (!childElement)
    {
      continue;
    }

    if (childElement->GetTagName() == "state")
    {
      LoadState(file, childElement, locator, entity, data);
    }
  }

}


void csSceneLoader::LoadState(const csCryoFile *file,
                              const csCryoFileElement *stateElement,
                              const csAssetLocator &locator,
                              csEntity *entity,
                              csSceneLoaderData *data) const
{
  if (!stateElement || !stateElement->HasAttribute("cls"))
  {
    // no class definition no loading
    return;
  }

  const csClass *cls = csClassRegistry::Get()->GetClass(stateElement->GetAttribute("cls", ""));
  if (!cls)
  {
    return;
  }


  for (const auto &loader: csEntityStateLoaderRegistry::Get().GetStateLoaders())
  {
    if (loader->CanLoad(cls))
    {
      csRef<csEntityState> state = loader->LoadState(file, stateElement, locator, data);
      if (state)
      {
        entity->AttachState(state);
      }
    }
  }

}


void csSceneLoader::LoadChildren(const csCryoFile *file,
                                 const csCryoFileElement *childrenElement,
                                 const csAssetLocator &locator,
                                 csEntity *entity,
                                 csSceneLoaderData *data) const
{

  for (int i = 0; i < childrenElement->GetNumberOfChildren(); ++i)
  {
    const csCryoFileElement *childElement = childrenElement->GetChild(i);
    if (!childElement)
    {
      continue;
    }

    if (childElement->GetTagName() == "entity")
    {

      csRef<csEntity> childEntity = LoadEntity(file, childElement, locator, data);
      entity->AttachEntity(childEntity);
    }
  }

}


} // cs