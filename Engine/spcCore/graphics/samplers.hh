

#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>
#include <spcCore/graphics/isampler.hh>

namespace spc
{

SPC_CLASS()
class SPC_CORE_API Samplers : public SPC_SUPER(iObject)
{
  SPC_CLASS_GEN_OBJECT;
public:
  Samplers();
  virtual ~Samplers();

  void SetDefault(iSampler * sampler);
  iSampler* GetDefault();

  void SetNormalMap(iSampler * sampler);
  iSampler* GetNormalMap();

  void SetUISprite(iSampler * sampler);
  iSampler* GetUISprite();


  void Load();

private:

  iSampler* m_default;
  iSampler* m_normalMap;
  iSampler* m_uiSprite;
  iSampler* m_offscreen;
  iSampler* m_offscreenDepth;

};

}