#pragma once


#include <ceCore/coreexport.hh>
#include <ceCore/types.hh>
#include <ceCore/math/color4f.hh>
#include <ceCore/math/matrix.hh>
#include <ceCore/class.hh>
#include <ceCore/graphics/eblendfactor.hh>
#include <ceCore/graphics/ecomparefunc.hh>
#include <ceCore/graphics/efillmode.hh>
#include <ceCore/graphics/erenderpass.hh>
#include <ceCore/graphics/etextureunit.hh>
#include <ceCore/graphics/idirectionallight.hh>
#include <ceCore/graphics/ipointlight.hh>
#include <ceCore/graphics/irendermesh.hh>
#include <ceCore/graphics/irendertarget.hh>
#include <ceCore/graphics/irendertarget2d.hh>
#include <ceCore/graphics/irendertarget2darray.hh>
#include <ceCore/graphics/irendertargetcube.hh>
#include <ceCore/graphics/isampler.hh>
#include <ceCore/graphics/itexture.hh>
#include <ceCore/graphics/itexture2d.hh>
#include <ceCore/graphics/itexture2darray.hh>
#include <ceCore/graphics/itexturecube.hh>
#include <ceCore/graphics/material/imaterial.hh>


namespace ce
{

struct iShader;

CE_CLASS()
struct CE_CORE_API iDevice : public CE_SUPER(iObject)
{
CE_CLASS_GEN;

  virtual ~iDevice()
  {}

  virtual void SetViewport(int16_t x, int16_t y, uint16_t width, uint16_t height) = 0;
  virtual void
  Clear(bool clearColor, const Color4f &color, bool clearDepth, float depth, bool clearStencil, uint8_t stencil) = 0;

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

  virtual void SetModelMatrix(const Matrix4f &modelMatrix) = 0;
  virtual void SetViewMatrix(const Matrix4f &viewMatrix) = 0;
  virtual void SetProjectionMatrix(const Matrix4f &projectionwMatrix) = 0;

  virtual void SetModelMatrix(const Matrix4f &modelMatrix, const Matrix4f &modelMatrixInv) = 0;
  virtual void SetViewMatrix(const Matrix4f &viewMatrix, const Matrix4f &viewMatrixInv) = 0;
  virtual void SetProjectionMatrix(const Matrix4f &projectionMatrix, const Matrix4f &projectionMatrixInv) = 0;

  virtual void SetShadowMapViewMatrices(const Matrix4f *viewMatrices, Size numMatrices) = 0;
  virtual void SetShadowMapProjectionMatrices(const Matrix4f *projectionMatrices, Size numMatrices) = 0;

  virtual const Matrix4f &GetViewMatrix() const = 0;
  virtual const Matrix4f &GetViewMatrixInv() const = 0;
  virtual const Matrix4f &GetProjectionMatrix() const = 0;
  virtual const Matrix4f &GetProjectionMatrixInv() const = 0;


  virtual Matrix4f &GetPerspectiveProjection(float l, float r, float b, float t, float n, float f, Matrix4f &m) = 0;
  virtual Matrix4f &GetPerspectiveProjectionInv(float l, float r, float b, float t, float n, float f, Matrix4f &m) = 0;
  virtual Matrix4f &GetOrthographicProjection(float l, float r, float b, float t, float n, float f, Matrix4f &m) = 0;
  virtual Matrix4f &GetOrthographicProjectionInv(float l, float r, float b, float t, float n, float f, Matrix4f &m) = 0;

  virtual void SetRenderLayer(int8_t renderLayer) = 0;
  virtual int8_t GetRenderLayer() const = 0;

  virtual void SetShader(iShader *shader) = 0;
  virtual void SetRenderTarget(iRenderTarget *target) = 0;
  virtual void SetRenderBuffer(uint32_t buffer) = 0;
  virtual void SetRenderBuffer(const std::vector<uint32_t> &buffer) = 0;

  virtual void ClearShadowMaps() = 0;
  virtual bool MoreShadowMapsPossible() const = 0;
  virtual void AddShadowMap(iTexture2D *shadowMap) = 0;
  virtual iTexture2D *GetShadowMap(unsigned idx) = 0;
  virtual void SetPointLightShadowMap(iLight *light,
                                      iTextureCube *colorMap,
                                      iTextureCube *depthMap,
                                      float near,
                                      float far,
                                      float bias) = 0;
  virtual void SetLightShadowMap(iLight *light, iTexture2D *shadowMap) = 0;

  virtual iSampler *CreateSampler() = 0;
  virtual iTexture2D *CreateTexture(const iTexture2D::Descriptor &descriptor) = 0;
  virtual iTexture2DArray *CreateTexture(const iTexture2DArray::Descriptor &descriptor) = 0;
  virtual iTextureCube *CreateTexture(const iTextureCube::Descriptor &descriptor) = 0;
  virtual iRenderTarget2D *CreateRenderTarget(const iRenderTarget2D::Descriptor &descriptor) = 0;
  virtual iRenderTarget2DArray *CreateRenderTarget(const iRenderTarget2DArray::Descriptor &descriptor) = 0;
  virtual iRenderTargetCube *CreateRenderTarget(const iRenderTargetCube::Descriptor &descriptor) = 0;
  virtual iDirectionalLight *CreateDirectionalLight() = 0;
  virtual iPointLight *CreatePointLight() = 0;


  virtual void ResetTextures() = 0;
  virtual void MarkTexture() = 0;
  virtual void ResetTexturesToMark() = 0;
  virtual eTextureUnit BindTexture(iTexture *texture) = 0;
  virtual bool BindMaterial(iMaterial *material, eRenderPass pass) = 0;
  virtual void Render(iRenderMesh *mesh, eRenderPass pass) = 0;
  virtual void RenderFullscreen() = 0;
  virtual void RenderFullscreen(iTexture2D *texture) = 0;
  virtual void RenderFullscreen(iTexture2DArray *texture, int layer) = 0;
  virtual void RenderFullscreen(iTextureCube *texture,
                                eCubeFace face,
                                const Vector2f &scale = Vector2f(1.0f, 1.0f),
                                const Vector2f &translation = Vector2f(0.0f, 0.0f)) = 0;

  virtual void BindForwardLight(const iLight *light, Size idx) = 0;
  virtual void FinishForwardLights(Size numLights) = 0;


#if _DEBUG
  virtual void ResetDebug () = 0;
  CE_NODISCARD virtual Size GetNumberOfDrawCalls() const = 0;
  CE_NODISCARD virtual Size GetNumberOfTriangles () const = 0;
  CE_NODISCARD virtual Size GetNumberOfShaderStateChanges () const = 0;
#endif


};

}