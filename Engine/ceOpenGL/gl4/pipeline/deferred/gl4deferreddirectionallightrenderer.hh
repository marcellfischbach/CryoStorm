//
// Created by Marcell on 18.09.2023.
//

#pragma once

namespace ce
{

struct iDevice;
struct iDirectionalLight;
struct iRenderTarget2D;
struct iShader;
struct iShaderAttribute;
class GBuffer;

namespace opengl
{
class GL4DeferredDirectionalLightRenderer
{
public:
  GL4DeferredDirectionalLightRenderer();

  void Initialize();
  void SetDevice (iDevice* device);

  void Render (iRenderTarget2D *target, GBuffer *gBuffer, iDirectionalLight * light);

private:
  iDevice *m_device;

  iShader *m_lightShader;
  iShaderAttribute *m_lightShaderLightColor;
  iShaderAttribute *m_lightShaderNegLightDirection;
  iShaderAttribute *m_lightShaderDiffuseRoughness;
  iShaderAttribute *m_lightShaderDepth;
  iShaderAttribute *m_lightShaderNormal;


};


}

}