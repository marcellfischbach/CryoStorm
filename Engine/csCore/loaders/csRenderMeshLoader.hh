//
// Created by MCEL on 24.02.2025.
//

#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/resource/csAssetLoader.hh>

namespace cs
{

struct iFile;
struct iRenderMesh;

class CS_CORE_API csRenderMeshLoader : public CS_SUPER(csAssetLoader)
{
public:
  csRenderMeshLoader ();
  ~csRenderMeshLoader() override = default;

  csOwned<iAsset> Load(const csAssetLocator &locator) const override;

private:
  csOwned<iAsset> LoadMesh (iFile* file, const csAssetLocator &locator) const;
  csOwned<iAsset> LoadRenderMesh (iFile* file) const;
  csOwned<iRenderMesh> ReadRenderMesh (iFile* file) const;
};

} // cs
