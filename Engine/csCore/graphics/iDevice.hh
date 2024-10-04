#pragma once


#include <csCore/csCoreExport.hh>
#include <csCore/csTypes.hh>
#include <csCore/math/csColor4f.hh>
#include <csCore/math/csMatrix.hh>
#include <csCore/csClass.hh>
#include <csCore/graphics/eBlendFactor.hh>
#include <csCore/graphics/eCompareFunc.hh>
#include <csCore/graphics/eFillMode.hh>
#include <csCore/graphics/eRenderPass.hh>
#include <csCore/graphics/eTextureUnit.hh>
#include <csCore/graphics/iDirectionalLight.hh>
#include <csCore/graphics/iPointLight.hh>
#include <csCore/graphics/iRenderMesh.hh>
#include <csCore/graphics/iRenderTarget.hh>
#include <csCore/graphics/iRenderTarget2D.hh>
#include <csCore/graphics/iRenderTarget2DArray.hh>
#include <csCore/graphics/iRenderTargetCube.hh>
#include <csCore/graphics/iSampler.hh>
#include <csCore/graphics/iTexture.hh>
#include <csCore/graphics/iTexture2D.hh>
#include <csCore/graphics/iTexture2DArray.hh>
#include <csCore/graphics/iTextureCube.hh>
#include <csCore/graphics/material/iMaterial.hh>
#include <array>

namespace cryo
{

struct iShader;

CS_CLASS(jclass = "org.crimsonedge.core.graphics.IDevice")
struct CS_CORE_API iDevice : public CS_SUPER(iObject)
{
CS_CLASS_GEN;

  virtual ~iDevice()
  {}

  virtual void SetViewport(int16_t x, int16_t y, uint16_t width, uint16_t height) = 0;
  virtual void Clear(bool clearColor,
                     const csColor4f &color,
                     bool clearDepth,
                     float depth,
                     bool clearStencil,
                     uint8_t stencil) = 0;

  virtual void SetColorWrite(bool redMask, bool greenMask, bool blueMask, bool alphaMask) = 0;
  virtual void SetDepthWrite(bool depthMask) = 0;
  virtual void SetDepthTest(bool depthTest) = 0;
  virtual void SetFillMode(eFillMode fillMode) = 0;
  virtual void SetDepthFunc(eCompareFunc func) = 0;

  virtual void SetBlending(bool blending) = 0;
  virtual void SetBlendFactor(eBlendFactor srcFactor, eBlendFactor dstFactor) = 0;
  virtual void SetBlendFactor(eBlendFactor srcFactorColor,
                              eBlendFactor srcFactorAlpha,
                              eBlendFactor dstFactorColor,
                              eBlendFactor dstFactorAlpha) = 0;

  virtual void SetModelMatrix(const csMatrix4f &modelMatrix) = 0;
  virtual void SetViewMatrix(const csMatrix4f &viewMatrix) = 0;
  virtual void SetProjectionMatrix(const csMatrix4f &projectionwMatrix) = 0;

  virtual void SetModelMatrix(const csMatrix4f &modelMatrix, const csMatrix4f &modelMatrixInv) = 0;
  virtual void SetViewMatrix(const csMatrix4f &viewMatrix, const csMatrix4f &viewMatrixInv) = 0;
  virtual void SetProjectionMatrix(const csMatrix4f &projectionMatrix, const csMatrix4f &projectionMatrixInv) = 0;

  virtual void SetShadowMapViewMatrices(const csMatrix4f *viewMatrices, Size numMatrices) = 0;
  virtual void SetShadowMapProjectionMatrices(const csMatrix4f *projectionMatrices, Size numMatrices) = 0;

  virtual void SetSkeletonMatrices(const csMatrix4f *skeletonMatrices, Size numMatrices) = 0;

  virtual const csMatrix4f &GetViewMatrix() const = 0;
  virtual const csMatrix4f &GetViewMatrixInv() const = 0;
  virtual const csMatrix4f &GetProjectionMatrix() const = 0;
  virtual const csMatrix4f &GetProjectionMatrixInv() const = 0;


  virtual csMatrix4f &GetPerspectiveProjection(float l, float r, float b, float t, float n, float f, csMatrix4f &m) = 0;
  virtual csMatrix4f &GetPerspectiveProjectionInv(float l, float r, float b, float t, float n, float f, csMatrix4f &m) = 0;
  virtual csMatrix4f &GetOrthographicProjection(float l, float r, float b, float t, float n, float f, csMatrix4f &m) = 0;
  virtual csMatrix4f &GetOrthographicProjectionInv(float l, float r, float b, float t, float n, float f, csMatrix4f &m) = 0;

  virtual void SetRenderLayer(int8_t renderLayer) = 0;
  virtual int8_t GetRenderLayer() const = 0;

  virtual void SetShader(iShader *shader) = 0;
  virtual void SetRenderTarget(iRenderTarget *target) = 0;
  virtual void SetRenderBuffer(uint32_t buffer) = 0;
  virtual void SetRenderBuffer(const std::vector<uint32_t> &buffer) = 0;

  virtual void ClearShadowMaps() = 0;
  virtual bool MoreShadowMapsPossible() const = 0;
//  virtual void AddShadowMap(iTexture2D *shadowMap) = 0;
//  virtual iTexture2D *GetShadowMap(unsigned idx) = 0;
  virtual void SetPointLightShadowMap(size_t lightIdx,
                                      iPointLight *light,
                                      iTextureCube *shadowBufferDepth,
                                      iTextureCube *shadowBufferColor,
                                      float near,
                                      float far,
                                      float bias) = 0;

  virtual void AddDirectionalLightShadow(iDirectionalLight *light,
                                         iTexture2D *shadowMap,
                                         iTexture2DArray *shadowBuffersDepth,
                                         iTexture2DArray *shadowBuffersColor,
                                         const std::array<float, 4> &layers,
                                         const std::array<csMatrix4f, 4> &matrices) = 0;
//  virtual void SetLightShadowMap(iLight *light, iTexture2D *shadowMap) = 0;

  virtual iSampler *CreateSampler() = 0;
  virtual iTexture2D *CreateTexture(const iTexture2D::Descriptor &descriptor) = 0;
  virtual iTexture2DArray *CreateTexture(const iTexture2DArray::Descriptor &descriptor) = 0;
  virtual iTextureCube *CreateTexture(const iTextureCube::Descriptor &descriptor) = 0;
  virtual iRenderTarget2D *CreateRenderTarget(const iRenderTarget2D::Descriptor &descriptor) = 0;
  virtual iRenderTarget2DArray *CreateRenderTarget(const iRenderTarget2DArray::Descriptor &descriptor) = 0;
  virtual iRenderTargetCube *CreateRenderTarget(const iRenderTargetCube::Descriptor &descriptor) = 0;
  virtual iDirectionalLight *CreateDirectionalLight() = 0;
  virtual iPointLight *CreatePointLight() = 0;


  virtual void ClearTextureCache() = 0;
  virtual void ResetTextures() = 0;
  virtual void MarkTexture() = 0;
  virtual void ResetTexturesToMark() = 0;
  virtual eTextureUnit BindTexture(iTexture *texture) = 0;
  virtual bool BindMaterial(iMaterial *material, eRenderPass pass) = 0;
  virtual void Render(iRenderMesh *mesh, eRenderPass pass) = 0;
  virtual void RenderPixel() = 0;
  virtual void RenderFullscreen() = 0;
  virtual void RenderFullscreen(iTexture2D *texture) = 0;
  virtual void RenderFullscreen(iTexture2DArray *texture, int layer) = 0;
  virtual void RenderFullscreen(iTextureCube *texture,
                                eCubeFace face,
                                const csVector2f &scale = csVector2f(1.0f, 1.0f),
                                const csVector2f &translation = csVector2f(0.0f, 0.0f)) = 0;

  virtual void BindForwardLight(const iLight *light, Size idx) = 0;
  virtual void FinishForwardLights(Size numLights) = 0;


#if _DEBUG
  virtual void ResetDebug() = 0;
  CS_NODISCARD virtual Size GetNumberOfDrawCalls() const = 0;
  CS_NODISCARD virtual Size GetNumberOfTriangles() const = 0;
  CS_NODISCARD virtual Size GetNumberOfShaderStateChanges() const = 0;
#endif


};

}