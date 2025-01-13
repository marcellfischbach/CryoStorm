
#include <csCore/graphics/csSamplers.hh>
#include <csCore/resource/csAssetManager.hh>

namespace cs
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

csAssetRef<iSampler>  &csSamplers::GetDefault()
{
  return m_default;
}

void csSamplers::SetNormalMap(iSampler* sampler)
{
  CS_SET(m_normalMap, sampler);
}

csAssetRef<iSampler>  &csSamplers::GetNormalMap()
{
  return m_normalMap;
}

void csSamplers::SetUISprite(iSampler* sampler)
{
  CS_SET(m_uiSprite, sampler);
}

csAssetRef<iSampler>  &csSamplers::GetUISprite()
{
  return m_uiSprite;
}

csAssetRef<iSampler>  &csSamplers::GetTest()
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

  m_default = csAssetManager::Get()->Load<iSampler>("/samplers/default.sampler");
  m_test = csAssetManager::Get()->Load<iSampler>("/samplers/test.sampler");
  m_normalMap = csAssetManager::Get()->Load<iSampler>("/samplers/normal_map.sampler");
  m_uiSprite = csAssetManager::Get()->Load<iSampler>("/samplers/ui_sprite.sampler");

}


}