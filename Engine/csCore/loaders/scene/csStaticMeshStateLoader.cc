//
// Created by Marcell on 13.03.2025.
//

#include <csCore/loaders/scene/csStaticMeshStateLoader.hh>
#include <csCore/entity/csStaticMeshState.hh>
#include <csCore/graphics/csMesh.hh>
#include <csCore/resource/csAssetManager.hh>

namespace cs
{

csStaticMeshStateLoader::csStaticMeshStateLoader()
    : csSpatialStateLoader(100)
{
  RegisterClass(csStaticMeshState::GetStaticClass());
}

csOwned<csEntityState> csStaticMeshStateLoader::LoadState(const csCryoFile *file,
                                                          const csCryoFileElement *stateElement,
                                                          const csAssetLocator &locator,
                                                          csSceneLoaderData *data) const
{
  const csCryoFileElement *meshElement = stateElement->GetChild("mesh");
  if (!meshElement || !meshElement->HasAttribute("locator"))
  {
    return nullptr;
  }


  csStaticMeshState *staticMesh = new csStaticMeshState();

  csAssetLocator meshLocator(locator, meshElement->GetAttribute("locator", ""));
  csRef<csMesh>  mesh = csAssetManager::Get()->Get<csMesh>(meshLocator);
  if (mesh)
  {
    staticMesh->SetMesh(mesh);
  }


  LoadSpatialState(stateElement, staticMesh);


  return staticMesh;
}


} // cs