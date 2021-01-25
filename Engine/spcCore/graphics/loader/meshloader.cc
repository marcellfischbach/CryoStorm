

#include <spcCore/graphics/loader/meshloader.hh>
#include <spcCore/graphics/mesh.hh>


namespace spc
{

MeshLoader::MeshLoader()
  : iAssetLoader()
{
  SPC_CLASS_GEN_CONSTR;
}


MeshLoader::~MeshLoader()
{

}


bool MeshLoader::CanLoad(const Class* cls, const ResourceLocator& locator) const
{
  return Mesh::GetStaticClass() == cls;
}

iObject* MeshLoader::Load(const Class* cls, const ResourceLocator& locator) const
{
  return nullptr;
}

}