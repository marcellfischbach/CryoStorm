
#include <ceCore/graphics/csSamplers.hh>
#include <ceCore/resource/assetmanager.hh>

namespace cryo
{


csSamplers::csSamplers()
  : m_default(nullptr)
  , m_normalMap(nullptr)
  , m_uiSprite(nullptr)
{

}


csSamplers::~csSamplers()
{
  CS_RELEASE(m_default);
  CS_RELEASE(m_normalMap);
  CS_RELEASE(m_uiSprite);
}

void csSamplers::SetDefault(iSampler* sampler)
{
  CS_SET(m_default, sampler);
}

iSampler* csSamplers::GetDefault()
{
  return m_default;
}

void csSamplers::SetNormalMap(iSampler* sampler)
{
  CS_SET(m_normalMap, sampler);
}

iSampler* csSamplers::GetNormalMap()
{
  return m_normalMap;
}

void csSamplers::SetUISprite(iSampler* sampler)
{
  CS_SET(m_uiSprite, sampler);
}

iSampler* csSamplers::GetUISprite()
{
  return m_uiSprite;
}

iSampler* csSamplers::GetTest()
{
  return m_test;
}

void csSamplers::SetTest(iSampler* mTest)
{
  m_test = mTest;
}

void csSamplers::Load()
{

  // mock the default stuff

  m_default = AssetManager::Get()->Load<iSampler>(ResourceLocator("/samplers/default.sampler"));
  m_test = AssetManager::Get()->Load<iSampler>(ResourceLocator("/samplers/test.sampler"));
  m_normalMap = AssetManager::Get()->Load<iSampler>(ResourceLocator("/samplers/normal_map.sampler"));
  m_uiSprite = AssetManager::Get()->Load<iSampler>(ResourceLocator("/samplers/ui_sprite.sampler"));

}


}