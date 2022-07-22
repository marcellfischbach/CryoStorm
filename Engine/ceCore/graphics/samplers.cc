
#include <ceCore/graphics/samplers.hh>
#include <ceCore/resource/assetmanager.hh>

namespace ce
{


Samplers::Samplers()
  : m_default(nullptr)
  , m_normalMap(nullptr)
  , m_uiSprite(nullptr)
{

}


Samplers::~Samplers()
{
  CE_RELEASE(m_default);
  CE_RELEASE(m_normalMap);
  CE_RELEASE(m_uiSprite);
}

void Samplers::SetDefault(iSampler* sampler)
{
  CE_SET(m_default, sampler);
}

iSampler* Samplers::GetDefault()
{
  return m_default;
}

void Samplers::SetNormalMap(iSampler* sampler)
{
  CE_SET(m_normalMap, sampler);
}

iSampler* Samplers::GetNormalMap()
{
  return m_normalMap;
}

void Samplers::SetUISprite(iSampler* sampler)
{
  CE_SET(m_uiSprite, sampler);
}

iSampler* Samplers::GetUISprite()
{
  return m_uiSprite;
}

iSampler* Samplers::GetTest()
{
  return m_test;
}

void Samplers::SetTest(iSampler* mTest)
{
  m_test = mTest;
}

void Samplers::Load()
{

  // mock the default stuff

  m_default = AssetManager::Get()->Load<iSampler>(ResourceLocator("/samplers/default.sampler"));
  m_test = AssetManager::Get()->Load<iSampler>(ResourceLocator("/samplers/test.sampler"));
  m_normalMap = AssetManager::Get()->Load<iSampler>(ResourceLocator("/samplers/normal_map.sampler"));
  m_uiSprite = AssetManager::Get()->Load<iSampler>(ResourceLocator("/samplers/ui_sprite.sampler"));

}


}