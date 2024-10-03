//
// Created by Marcell on 16.04.2024.
//

#include <ceAssimpLoader/assimpmaterialloader.hh>
#include <ceCore/resource/assetmanager.hh>
#include <ceCore/graphics/material/materialinstance.hh>
#include <assimp/scene.h>

namespace cryo::assimp
{

iMaterial *AssimpMaterialLoader::Read(aiMaterial *material)
{
  Material *baseMaterial = AssetManager::Get()->Get<Material>("file:///materials/Default.mat");
  if (!baseMaterial)
  {
    return nullptr;
  }
  
  
  MaterialInstance *result = new MaterialInstance();
  result->SetMaterial(baseMaterial);

  aiColor3D  diffuse;
  if (material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse) == aiReturn_SUCCESS)
  {
    Size idx = result->IndexOf("Color");
    if (idx != Material::UndefinedIndex)
    {
      result->Set(idx, Color4f(diffuse.r, diffuse.g, diffuse.b, 1.0f));
    }
  }
  return result;
}

}