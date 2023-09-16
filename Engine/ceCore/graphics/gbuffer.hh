#pragma  once

#include <ceCore/coreexport.hh>
#include <ceCore/types.hh>


namespace ce
{

struct iDevice;
struct iRenderTarget2D;
struct iSampler;
struct iTexture2D;

class CE_CORE_API GBuffer
{
public:
  GBuffer ();

  bool Update (iDevice* device, uint16_t width, uint16_t height);


private:
  void UpdateSamplers (iDevice *device);

  iSampler *m_diffuseRoughnessSampler;
  iSampler *m_depthSampler;
  iSampler *m_normalSampler;
  iSampler *m_emissionMetallicSampler;

  iRenderTarget2D *m_gBuffer;;
  iTexture2D *m_diffuseRoughness;
  iTexture2D *m_depth;
  iTexture2D *m_normal;
  iTexture2D *m_emissionMetallic;
};


}
