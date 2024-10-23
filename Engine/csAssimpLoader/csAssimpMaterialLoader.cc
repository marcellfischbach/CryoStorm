//
// Created by Marcell on 16.04.2024.
//

#include <csAssimpLoader/csAssimpMaterialLoader.hh>
#include <csCore/resource/csAssetManager.hh>
#include <csCore/graphics/material/csMaterialInstance.hh>
#include <assimp/scene.h>

namespace cs::assimp
{

iMaterial *csAssimpMaterialLoader::Read(aiMaterial *material)
{
  csMaterial *baseMaterial = csAssetManager::Get()->Get<csMaterial>("file:///materials/Default.mat");
  if (!baseMaterial)
  {
    return nullptr;
  }
  
  
  csMaterialInstance *result = new csMaterialInstance();
  result->SetMaterial(baseMaterial);

  aiColor3D  diffuse;
  if (material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse) == aiReturn_SUCCESS)
  {
    Size idx = result->IndexOf("Color");
    if (idx != csMaterial::UndefinedIndex)
    {
      result->SetColor4f(idx, csColor4f(diffuse.r, diffuse.g, diffuse.b, 1.0f));
    }
  }
  return result;
}

}