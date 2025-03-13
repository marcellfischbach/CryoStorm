//
// Created by Marcell on 13.03.2025.
//

#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/loaders/scene/csSpatialStateLoader.hh>

namespace cs
{


class CS_CORE_API csStaticMeshStateLoader : public csSpatialStateLoader
{
public:
  csStaticMeshStateLoader();
  csOwned<csEntityState> LoadState(const csCryoFile *file,
                                   const csCryoFileElement *stateElement,
                                   const csAssetLocator &locator,
                                   csSceneLoaderData *data) const override;


};

} // cs
