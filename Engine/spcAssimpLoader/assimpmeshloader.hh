
#pragma once

#include <spcAssimpLoader/assimpexport.hh>
#include <spcCore/resource/iassetloader.hh>
#include <spcCore/math/matrix4f.hh>
#include <png.h>

class aiMesh;
class aiNode;

namespace spc::assimp
{

struct LoaderData;

SPC_CLASS()
class SPC_ASSIMP_API AssimpMeshLoader : public SPC_SUPER(iAssetLoader)
{
  SPC_CLASS_GEN_OBJECT;
public:
  AssimpMeshLoader();
  virtual ~AssimpMeshLoader() { }

  virtual bool CanLoad(const Class * cls, const ResourceLocator & locator) const;

  virtual iObject* Load(const Class * cls, const ResourceLocator & locator) const;

private:
  void ReadNode(aiNode* node, const Matrix4f &parentMatrix, LoaderData& d) const;

  
};


}