
#include <spcCore/loaders/materialloader.hh>
#include <spcCore/graphics/material/material.hh>
#include <spcCore/graphics/material/materialinstance.hh>

namespace spc
{


bool MaterialLoaderSpc::CanLoad(const Class *cls, const file::File *file, const ResourceLocator *locator) const
{
  return cls == Material::GetStaticClass() && file && file->Root()->HasChild("material")
  || cls == MaterialInstance::GetStaticClass() && file && file->Root()->HasChild("materialinstance");
}

iObject *MaterialLoaderSpc::Load(const Class *cls, const file::File *file, const ResourceLocator *locator) const
{
  if (cls == Material::GetStaticClass() && file && file->Root()->HasChild("material"))
  {
    return LoadMaterial(cls, file, locator);
  }
  else if (cls == MaterialInstance::GetStaticClass() && file && file->Root()->HasChild("materialinstance"))
  {
    return LoadMaterialInstance(cls, file, locator);
  }
  return nullptr;
}


iObject *MaterialLoaderSpc::LoadMaterial(const Class *cls, const file::File *file, const ResourceLocator *locator) const
{
  return nullptr;
}

iObject *MaterialLoaderSpc::LoadMaterialInstance(const Class *cls, const file::File *file, const ResourceLocator *locator) const
{
  return nullptr;
}


}