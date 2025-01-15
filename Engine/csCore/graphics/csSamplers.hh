

#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/graphics/iSampler.hh>
#include <csCore/csRef.hh>

namespace cs
{

CS_CLASS()
class CS_CORE_API csSamplers : public CS_SUPER(iObject)
{
  CS_CLASS_GEN_OBJECT;
public:
  csSamplers();
  virtual ~csSamplers();

  void SetDefault(iSampler * sampler);
  iSampler* GetDefault();

  void SetNormalMap(iSampler * sampler);
  iSampler* GetNormalMap();

  void SetUISprite(iSampler * sampler);
  iSampler* GetUISprite();

  void SetTest(iSampler *sampler);
  iSampler* GetTest();

  void Load();

private:

  csAssetRef<iSampler> m_default;
  csAssetRef<iSampler> m_normalMap;
  csAssetRef<iSampler> m_uiSprite;
  csAssetRef<iSampler> m_test;
  csAssetRef<iSampler> m_offscreen;
  csAssetRef<iSampler> m_offscreenDepth;

};

}