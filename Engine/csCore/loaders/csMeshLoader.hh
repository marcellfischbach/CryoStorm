
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/resource/csBaseCSFAssetLoader.hh>

namespace cs
{

class csBinaryInputStream;
struct iRenderMesh;

CS_CLASS()
class csMeshLoader : public CS_SUPER(csBaseCSFAssetLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  csMeshLoader();
  ~csMeshLoader() override = default;

protected:
  csOwned<iAsset> Load(const file::csCryoFile *file, const csAssetLocator &locator) const override ;
  csOwned<iRenderMesh> ReadRenderMesh(csBinaryInputStream & is) const;
  csOwned<iRenderMesh> ReadRenderMesh_V1(csBinaryInputStream & is) const;
};

} // cs
