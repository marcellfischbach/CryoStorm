

#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/graphics/iSampler.hh>

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

  iSampler* m_default;
  iSampler* m_normalMap;
  iSampler* m_uiSprite;
  iSampler* m_test;
  iSampler* m_offscreen;
  iSampler* m_offscreenDepth;

};

}