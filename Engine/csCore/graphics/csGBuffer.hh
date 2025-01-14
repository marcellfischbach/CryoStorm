#pragma  once

#include <csCore/csCoreExport.hh>
#include <csCore/csTypes.hh>
#include <csCore/csRef.hh>
#include <vector>

namespace cs
{

struct iDevice;
struct iRenderTarget2D;
struct iSampler;
struct iTexture2D;

class CS_CORE_API csGBuffer
{
public:
  csGBuffer();

  bool Update (iDevice* device, uint16_t width, uint16_t height);
  iRenderTarget2D *GetGBuffer() const;
  iTexture2D *GetDiffuseRoughness() const;
  iTexture2D *GetDepth() const;
  iTexture2D *GetNormal() const;
  iTexture2D *GetEmissionMetallic() const;

  const std::vector<uint32_t> &GetBufferIDs () const;

private:
  void UpdateSamplers (iDevice *device);

  csAssetRef<iSampler>  m_diffuseRoughnessSampler;
  csAssetRef<iSampler>  m_depthSampler;
  csAssetRef<iSampler>  m_normalSampler;
  csAssetRef<iSampler>  m_emissionMetallicSampler;


  std::vector<uint32_t> m_bufferIds;
  iRenderTarget2D *m_gBuffer;
  csAssetRef<iTexture2D> m_diffuseRoughness;
  csAssetRef<iTexture2D> m_depth;
  csAssetRef<iTexture2D> m_normal;
  csAssetRef<iTexture2D> m_emissionMetallic;
};


}
