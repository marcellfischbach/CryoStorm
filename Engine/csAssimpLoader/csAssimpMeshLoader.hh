
#pragma once

#include <csAssimpLoader/csAssimpExport.hh>
#include <csCore/resource/iAssetLoader.hh>
#include <csCore/math/csMatrix4f.hh>

struct aiMesh;
struct aiNode;
struct aiMaterial;

namespace cs
{
struct iMaterial;
}

namespace cs::assimp
{

struct StaticLoaderData;

CS_CLASS()
class CS_ASSIMP_API csAssimpMeshLoader : public CS_SUPER(iAssetLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  csAssimpMeshLoader();
  virtual ~csAssimpMeshLoader() { }

  virtual bool CanLoad(const csClass * cls, const csAssetLocator & locator) const;

  virtual iObject* Load(const csClass * cls, const csAssetLocator & locator) const;

private:
  void ReadNode(aiNode* node, const csMatrix4f &parentMatrix, StaticLoaderData& d) const;

};


}