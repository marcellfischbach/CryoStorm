
#pragma  once

#include <csCore/math/csVector.hh>
#include <csCore/csRef.hh>
namespace cs
{

struct iShader;
struct iShaderAttribute;
struct iTexture2D;

namespace opengl
{

class csGL4Device;
class csGL4RenderTarget2D;

class csGL4TwinPSSMFilter
{
public:




  csGL4TwinPSSMFilter();
  ~csGL4TwinPSSMFilter();

  bool Initialize(const csVector2f &distance, float radius, float samplesFactor, float maxSampleDistance);

  void Render(csGL4Device* device, iTexture2D* depthBuffer, iTexture2D *shadowMap, csGL4RenderTarget2D *target);


private:
  csVector2f m_distance;
  float      m_radius;
  float m_samplesFactor;
  float m_maxSampleDistance;

  csAssetRef<iShader> m_shadowMapFilterShader = nullptr;
  iShaderAttribute *m_attrFilterDepthBuffer = nullptr;
  iShaderAttribute *m_attrFilterShadowMap = nullptr;
  iShaderAttribute *m_attrFilterRadius = nullptr;
  iShaderAttribute *m_attrFilterDistance = nullptr;
  iShaderAttribute *m_attrFilterSamplesFactor = nullptr;
  iShaderAttribute *m_attrFilterMaxSampleDistance = nullptr;
  iShaderAttribute *m_attrScreenAspect = nullptr;
  iShaderAttribute *m_attrScreenSize = nullptr;
};

}

}
