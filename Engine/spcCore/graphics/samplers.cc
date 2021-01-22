
#include <spcCore/graphics/samplers.hh>
#include <spcCore/objectregistry.hh>
#include <spcCore/graphics/idevice.hh>

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
  iDevice *device = ObjectRegistry::Get<iDevice>();
  m_default = device->CreateSampler();
  m_default->SetFilterMode(eFM_MinMagLinear);
  m_default->SetAnisotropy(1);

  m_normalMap = device->CreateSampler();
  m_normalMap->SetFilterMode(eFM_MinMagLinear);
  m_normalMap->SetAnisotropy(1);

  m_uiSprite = device->CreateSampler();
  m_uiSprite->SetFilterMode(eFM_MinMagNearest);
  m_uiSprite->SetAnisotropy(1);
  /*
  m_uiSprite->SetAddressU(eTAM_Clamp);
  m_uiSprite->SetAddressV(eTAM_Clamp);
  m_uiSprite->SetAddressW(eTAM_Clamp);
  */
}


}