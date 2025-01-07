//
// Created by Marcell on 07.05.2024.
//

#include "exitgamestate.hh"
#include <csCore/input/csInput.hh>
#include <csCore/csEngine.hh>
#include <csCore/graphics/material/iMaterial.hh>
#include <csCore/resource/csAssetManager.hh>
#include <csCore/resource/csResourcePool.hh>

ExitGameState::ExitGameState()
{
  SetNeedUpdate(true);

  m_material0 = cs::csAssetManager::Get()->Load<cs::iMaterial>("/materials/DefaultBlue.mat");
  m_material1 = cs::csAssetManager::Get()->Load<cs::iMaterial>("/materials/DefaultRed.mat");
  m_material0->SetLocator(cs::csResourceLocator("/materials/Default.mat"));
  m_material1->SetLocator(cs::csResourceLocator("/materials/Default.mat"));
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
      cs::csResourcePool::Instance().Put(m_material0);
    }
    else
    {
      cs::csResourcePool::Instance().Put(m_material1);
    }
    set = !set;
  }
}
