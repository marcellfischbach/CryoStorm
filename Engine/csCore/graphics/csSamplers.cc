
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
}

void csSamplers::SetDefault(iSampler* sampler)
{
  m_default = sampler;
}

iSampler* csSamplers::GetDefault()
{
  return m_default;
}

void csSamplers::SetNormalMap(iSampler* sampler)
{
  m_normalMap = sampler;
}

iSampler* csSamplers::GetNormalMap()
{
  return m_normalMap;
}

void csSamplers::SetUISprite(iSampler* sampler)
{
  m_uiSprite =  sampler;
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

  m_default = csAssetManager::Get()->Load<iSampler>("/samplers/Default.sampler");
  m_test = csAssetManager::Get()->Load<iSampler>("/samplers/test.sampler");
  m_normalMap = csAssetManager::Get()->Load<iSampler>("/samplers/normal_map.sampler");
  m_uiSprite = csAssetManager::Get()->Load<iSampler>("/samplers/ui_sprite.sampler");

}


}