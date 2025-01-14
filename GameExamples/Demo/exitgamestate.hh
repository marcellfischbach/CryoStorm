//
// Created by Marcell on 07.05.2024.
//

#pragma once

#include <csCore/entity/csEntityState.hh>
#include <csCore/csRef.hh>

namespace cs
{
struct iMaterial;
}

CS_CLASS()
class ExitGameState : public CS_SUPER(cs::csEntityState)
{
  CS_CLASS_GEN;
public:
  ExitGameState ();


  void Update(float tpf) override;

private:
  cs::csAssetRef<cs::iMaterial> m_material0;
  cs::csAssetRef<cs::iMaterial> m_material1;

};


