#pragma once

#include <csCore/csRef.hh>
#include <csCore/math/csMatrix4f.hh>

namespace cs
{

class csCamera;
class csProjector;
struct iGfxScene;
struct iTexture2D;

namespace opengl
{

class csGL4Device;
class csGL4DirectionalLight;
class csGL4RenderTarget2D;

struct iPSSMShadowBufferObject
{

};


struct iPSSMRenderer
{
  virtual void SetDepthBuffer(iTexture2D *depthBuffer) = 0;
  virtual void SetDevice(csGL4Device *device) = 0;
  virtual void SetScene(iGfxScene *scene) = 0;
  virtual csOwned<csGL4RenderTarget2D> CreateDirectionalLightShadowMap() = 0;

  virtual void SetShadowMap(csGL4RenderTarget2D *shadowMap) = 0;
  virtual csGL4RenderTarget2D *GetShadowMap() = 0;


  virtual iPSSMShadowBufferObject *CreateDirectionalLightShadowBuffer() = 0;
  virtual void DeleteDirectionalLightShadowBuffer(iPSSMShadowBufferObject *sbo) = 0;

  virtual void SetShadowBuffer(iPSSMShadowBufferObject *shadowBuffer) = 0;


  virtual void RenderShadow(const csGL4DirectionalLight *directionalLight,
                            const csCamera &camera,
                            const csProjector &projector) = 0;

  virtual bool IsShadowMapValid(csGL4RenderTarget2D *shadowMap) const = 0;
  virtual bool IsShadowBufferValid(iPSSMShadowBufferObject *shadowMap) const = 0;


};


}
}