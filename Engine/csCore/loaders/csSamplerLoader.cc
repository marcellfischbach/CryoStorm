

#include <csCore/loaders/csSamplerLoader.hh>
#include <csCore/graphics/iSampler.hh>
#include <csCore/graphics/iDevice.hh>
#include <csCore/csObjectRegistry.hh>


namespace cs
{

using namespace file;

csSamplerLoader::csSamplerLoader()
: csBaseCSFAssetLoader()
{
  RegisterType("SAMPLER");
}


#define IF(prefix, name, text) if (std::string(#name) == (text)) return prefix##name

eFilterMode FilterMode(const std::string& filterMode)
{
#define IF_(name) IF(eFM_, name, filterMode)
#define _IF_(name) else IF(eFM_, name, filterMode)
  IF_(MinMagNearest);
  _IF_(MinNearestMagLinear);
  _IF_(MinLinearMagNearest);
  _IF_(MinMagLinear);
  _IF_(MinMagMipNearest);
  _IF_(MinMagNearestMipLinear);
  _IF_(MinNearestMagLinearMipNearest);
  _IF_(MinNearestMagMipLinear);
  _IF_(MinLinearMagMipNearest);
  _IF_(MinLinearMagNearestMipLinear);
  _IF_(MinMagLinearMipNearest);
  _IF_(MinMagMipLinear);
  _IF_(Anisotropic);
#undef IF_
#undef _IF_
  return eFM_MinMagNearest;
}


eTextureAddressMode AddressMode(const std::string& addressMode)
{

#define IF_(name) IF(eTAM_, name, addressMode)
#define _IF_(name) else IF(eTAM_, name, addressMode)
  IF_(Repeat);
  _IF_(Clamp);
  _IF_(Mirror);
  _IF_(MirrorOnce);
#undef IF_
#undef _IF_
  return eTAM_Repeat;
}

eTextureCompareMode TextureCompareMode(const std::string& compareMode)
{

#define IF_(name) IF(eTCM_, name, compareMode)
#define _IF_(name) else IF(eTCM_, name, compareMode)
  IF_(CompareToR);
  _IF_(None);
#undef IF_
#undef _IF_
  return eTCM_None;
}

eCompareFunc CompareFunc(const std::string& compareFunc)
{

#define IF_(name) IF(eCF_, name, compareFunc)
#define _IF_(name) else IF(eCF_, name, compareFunc)
  IF_(Less);
  _IF_(LessOrEqual);
  _IF_(Greater);
  _IF_(GreaterOrEqual);
  _IF_(Equal);
  _IF_(NotEqual);
  _IF_(Always);
  _IF_(Never);
#undef IF_
#undef _IF_
  return eCF_Always;
}

csOwned<iAsset> csSamplerLoader::Load(const csCryoFile *file, const csAssetLocator &locator) const
{
  const csCryoFileElement * samplerElement = file->Root()->GetChild("sampler");
  if (!samplerElement)
  {
    return nullptr;
  }

  eFilterMode filter = FilterMode(samplerElement->GetAttribute("filter", ""));
  int anisotropy = samplerElement->GetAttribute("anisotropy", 1);
  int minLOD = samplerElement->GetAttribute("minLOD", -1000);
  int maxLOD = samplerElement->GetAttribute("maxLOD", 1000);
  eTextureAddressMode addressU = AddressMode(samplerElement->GetAttribute("addressU", ""));
  eTextureAddressMode addressV = AddressMode(samplerElement->GetAttribute("addressV", ""));
  eTextureAddressMode addressW = AddressMode(samplerElement->GetAttribute("addressW", ""));
  std::string borderColor = samplerElement->GetAttribute("borderColor", "#000000");
  eTextureCompareMode compareMode = TextureCompareMode(samplerElement->GetAttribute("textureCompareMode", ""));
  eCompareFunc compareFunc = CompareFunc(samplerElement->GetAttribute("textureCompareFunc", ""));


  auto sampler = csObjectRegistry::Get<iDevice>()->CreateSampler();
  if (sampler)
  {
    sampler->SetFilterMode(filter);
    sampler->SetAnisotropy(anisotropy);
    sampler->SetMinLOD((int16_t)minLOD);
    sampler->SetMaxLOD((int16_t)maxLOD);
    sampler->SetAddressU(addressU);
    sampler->SetAddressV(addressV);
    sampler->SetAddressW(addressW);
    sampler->SetTextureCompareMode(compareMode);
    sampler->SetTextureCompareFunc(compareFunc);


  }
  


  return sampler;
}



}