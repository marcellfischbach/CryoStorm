#pragma  once

#include <ceCore/coreexport.hh>
#include <ceCore/types.hh>
#include <vector>

namespace cryo
{

struct iDevice;
struct iRenderTarget2D;
struct iSampler;
struct iTexture2D;

class CS_CORE_API GBuffer
{
public:
  GBuffer ();

  bool Update (iDevice* device, uint16_t width, uint16_t height);
  iRenderTarget2D *GetGBuffer() const;
  iTexture2D *GetDiffuseRoughness() const;
  iTexture2D *GetDepth() const;
  iTexture2D *GetNormal() const;
  iTexture2D *GetEmissionMetallic() const;

  const std::vector<uint32_t> &GetBufferIDs () const;

private:
  void UpdateSamplers (iDevice *device);

  iSampler *m_diffuseRoughnessSampler;
  iSampler *m_depthSampler;
  iSampler *m_normalSampler;
  iSampler *m_emissionMetallicSampler;


  std::vector<uint32_t> m_bufferIds;
  iRenderTarget2D *m_gBuffer;
  iTexture2D *m_diffuseRoughness;
  iTexture2D *m_depth;
  iTexture2D *m_normal;
  iTexture2D *m_emissionMetallic;
};


}
