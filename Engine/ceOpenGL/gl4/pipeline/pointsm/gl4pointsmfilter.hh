
#pragma  once

#include <ceCore/math/csVector.hh>

namespace cryo
{

struct iShader;
struct iShaderAttribute;
struct iTexture2D;

namespace opengl
{

class GL4Device;
class GL4RenderTarget2D;

class GL4PointSMFilter
{
public:




  GL4PointSMFilter();
  ~GL4PointSMFilter();

  bool Initialize(const csVector2f &distance, float radius, float samples, float maxSampleDistance);

  void Render (GL4Device* device, iTexture2D* depthBuffer, iTexture2D *shadowMap, GL4RenderTarget2D *target);


private:
  csVector2f m_distance;
  float      m_radius;
  float m_samples;
  float m_maxSampleDistance;

  iShader *m_shadowMapFilterShader = nullptr;
  iShaderAttribute *m_attrFilterDepthBuffer = nullptr;
  iShaderAttribute *m_attrFilterShadowMap = nullptr;
  iShaderAttribute *m_attrFilterRadius = nullptr;
  iShaderAttribute *m_attrFilterDistance = nullptr;
  iShaderAttribute *m_attrFilterSamples = nullptr;
  iShaderAttribute *m_attrFilterMaxSampleDistance = nullptr;
  iShaderAttribute *m_attrScreenAspect = nullptr;


};

}

}
