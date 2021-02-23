
#include <spcCore/graphics/samplers.hh>
#include <spcCore/resource/assetmanager.hh>

namespace spc
{


Samplers::Samplers()
  : m_default(nullptr)
  , m_normalMap(nullptr)
  , m_uiSprite(nullptr)
{

}


Samplers::~Samplers()
{
  SPC_RELEASE(m_default);
  SPC_RELEASE(m_normalMap);
  SPC_RELEASE(m_uiSprite);
}

void Samplers::SetDefault(iSampler* sampler)
{
  SPC_SET(m_default, sampler);
}

iSampler* Samplers::GetDefault()
{
  return m_default;
}

void Samplers::SetNormalMap(iSampler* sampler)
{
  SPC_SET(m_normalMap, sampler);
}

iSampler* Samplers::GetNormalMap()
{
  return m_normalMap;
}

void Samplers::SetUISprite(iSampler* sampler)
{
  SPC_SET(m_uiSprite, sampler);
}

iSampler* Samplers::GetUISprite()
{
  return m_uiSprite;
}


void Samplers::Load()
{

  // mock the default stuff

  m_default = AssetManager::Get()->Load<iSampler>(ResourceLocator("/samplers/default.spc"));
  m_normalMap = AssetManager::Get()->Load<iSampler>(ResourceLocator("/samplers/normal_map.spc"));
  m_uiSprite = AssetManager::Get()->Load<iSampler>(ResourceLocator("/samplers/ui_sprite.spc"));

}


}