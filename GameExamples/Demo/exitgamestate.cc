//
// Created by Marcell on 07.05.2024.
//

#include "exitgamestate.hh"
#include <csCore/input/csInput.hh>
#include <csCore/csEngine.hh>
#include <csCore/graphics/material/iMaterial.hh>
#include <csCore/resource/csAssetManager.hh>
#include <csCore/resource/csAssetPool.hh>

ExitGameState::ExitGameState()
{
  SetNeedUpdate(true);



}

void ExitGameState::Update(float tpf)
{
  if (cs::csInput::IsKeyPressed(cs::eKey::eK_Escape))
  {
    cs::csEngine::Get()->Exit(0);
  }

  static bool set = false;
  if (cs::csInput::IsKeyPressed(cs::eKey::eK_P))
  {

    if (set)
    {
      cs::iMaterial* mat = cs::csAssetManager::Get()->Load<cs::iMaterial>("/materials/DefaultBlue.mat");
      mat->SetLocator(cs::csAssetLocator("/materials/Default.mat"));
      cs::csAssetPool::Instance().Put(mat);
    }
    else
    {
      cs::iMaterial* mat = cs::csAssetManager::Get()->Load<cs::iMaterial>("/materials/DefaultRed.mat");
      mat->SetLocator(cs::csAssetLocator("/materials/Default.mat"));
      cs::csAssetPool::Instance().Put(mat);
    }
    set = !set;
  }
}
