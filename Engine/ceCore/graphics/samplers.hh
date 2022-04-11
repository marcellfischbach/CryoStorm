

#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/graphics/isampler.hh>

namespace ce
{

CE_CLASS()
class CE_CORE_API Samplers : public CE_SUPER(iObject)
{
  CE_CLASS_GEN_OBJECT;
public:
  Samplers();
  virtual ~Samplers();

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