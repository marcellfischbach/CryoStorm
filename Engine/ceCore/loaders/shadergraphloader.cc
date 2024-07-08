//
// Created by Marcell on 07.07.2024.
//

#include <ceCore/loaders/shadergraphloader.hh>
#include <ceCore/resource/vfs.hh>
#include <ceCore/graphics/shadergraph/shadergraph.hh>

namespace ce
{

bool ShaderGraphLoader::CanLoad(const Class *cls, const ResourceLocator &locator) const
{
  return cls == ShaderGraph::GetStaticClass()
         && (locator.GetExtension() == "SG"
             || locator.GetExtension() == "SHADERGRAPH");
}


iObject *ShaderGraphLoader::Load(const Class *cls, const ResourceLocator &locator) const
{
  iFile *file = VFS::Get()->Open(locator, eAM_Read, eOM_Binary);
  if (!file)
  {
    return nullptr;
  }

  CrimsonFile cFile;
  if (!cFile.Parse(file))
  {
    file->Release();
    return nullptr;
  }



  return nullptr;
}

} // ce