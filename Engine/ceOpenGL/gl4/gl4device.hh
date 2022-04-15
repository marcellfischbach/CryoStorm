
#pragma once

#include <ceOpenGL/openglexport.hh>
#include <ceCore/class.hh>
#include <ceCore/graphics/idevice.hh>
#include <ceCore/graphics/etextureunit.hh>
#include <map>

namespace ce::opengl
{

class GL4Program;

CE_CLASS()
class CE_OGL_API GL4Device : public CE_SUPER(iDevice)
{
  CE_CLASS_GEN_OBJECT;
public:
  GL4Device();
  virtual ~GL4Device();

  bool Initialize();

  void SetViewport(int16_t x, int16_t y, uint16_t width, uint16_t height) override;
  void Clear(bool clearColor, const Color4f & color, bool clearDepth, float depth, bool clearStencil, uint8_t stencil) override;

  void SetColorWrite(bool redWrite, bool greenWrite, bool blueWrite, bool alphaWrite) override;
  void SetDepthWrite(bool depthWrite) override;
  void SetDepthTest(bool depthTest) override;
  void SetBlending(bool blending) override;
  void SetBlendFactor(eBlendFactor srcFactor, eBlendFactor dstFactor) override;
  void SetBlendFactor(eBlendFactor srcFactorColor,
                      eBlendFactor srcFactorAlpha,
                      eBlendFactor dstFactorColor,
                      eBlendFactor dstFactorAlpha) override;

  void SetModelMatrix(const Matrix4f & modelMatrix) override;
  void SetViewMatrix(const Matrix4f & viewMatrix) override;
  void SetProjectionMatrix(const Matrix4f & projectionwMatrix) override;

  void SetModelMatrix(const Matrix4f& modelMatrix, const Matrix4f& modelMatrixInv) override;
  void SetViewMatrix(const Matrix4f& viewMatrix, const Matrix4f& viewMatrixInv) override;
  void SetProjectionMatrix(const Matrix4f& projectionwMatrix, const Matrix4f &projectionMatrixInv) override;

  void SetShadowMapViewMatrices(const Matrix4f * viewMatrices, Size numMatrices) override;
  void SetShadowMapProjectionMatrices(const Matrix4f * projectionMatrices, Size numMatrices) override;

  const Matrix4f &GetViewMatrix() const override;
  const Matrix4f &GetViewMatrixInv() const override;
  const Matrix4f &GetProjectionMatrix() const override;
  const Matrix4f &GetProjectionMatrixInv() const override;

  Matrix4f& GetPerspectiveProjection(float l, float r, float b, float t, float n, float f, Matrix4f & m) override;
  Matrix4f& GetPerspectiveProjectionInv(float l, float r, float b, float t, float n, float f, Matrix4f & m) override;
  Matrix4f& GetOrthographicProjection(float l, float r, float b, float t, float n, float f, Matrix4f & m) override;
  Matrix4f& GetOrthographicProjectionInv(float l, float r, float b, float t, float n, float f, Matrix4f & m) override;


  void SetShader(iShader * shader) override;
  void SetRenderTarget(iRenderTarget * target) override;

  void ClearShadowMaps() override;
  void SetPointLightShadowMap(iLight * light, iTextureCube * colorMap, iTextureCube * depthMap, float near, float far, float bias) override;
  void SetDirectionalLightShadowMap(iLight * light, const Vector3f &layers, iTexture2DArray * colorMap, iTexture2DArray * depthMap, Matrix4f matrices[3], float bias) override;


  iSampler *CreateSampler() override;
  iTexture2D* CreateTexture(const iTexture2D::Descriptor & descriptor) override;
  iTexture2DArray* CreateTexture(const iTexture2DArray::Descriptor & descriptor) override;
  iTextureCube* CreateTexture(const iTextureCube::Descriptor & descriptor) override;
  iRenderTarget2D* CreateRenderTarget(const iRenderTarget2D::Descriptor & descriptor) override;
  iRenderTarget2DArray* CreateRenderTarget(const iRenderTarget2DArray::Descriptor & descriptor) override;
  iRenderTargetCube* CreateRenderTarget(const iRenderTargetCube::Descriptor & descriptor) override;
  iDirectionalLight* CreateDirectionalLight() override;
  iPointLight* CreatePointLight() override;


  void ResetTextures() override;
  void MarkTexture() override;
  void ResetTexturesToMark() override;
  void SetSampler(eTextureUnit unit, iSampler * sampler);
  eTextureUnit BindTexture(iTexture * texture) override;
  bool BindMaterial(iMaterial * material, eRenderPass pass) override;
  void Render(iRenderMesh * mesh, eRenderPass pass) override;
  void RenderFullscreen(iTexture2D * texture) override;
  void RenderFullscreen(iTexture2DArray * texture, int layer) override;
  void RenderFullscreen(iTextureCube * texture, int layer) override;

  void BindForwardLight(const iLight * light, Size idx);
  void FinishForwardLights(Size numLights);

  void BindMatrices();

#if _DEBUG


  void IncDrawCalls();
  void IncTriangles(Size num);

  void ResetDebug() override;
  CE_NODISCARD Size GetNumberOfDrawCalls() const override;
  CE_NODISCARD Size GetNumberOfTriangles () const override;
  CE_NODISCARD Size GetNumberOfShaderStateChanges () const override;
#endif

private:
  void UnbindAllTextures();

private:
  iRenderTarget* m_renderTarget;
  iShader* m_shader;
  iMaterial* m_material;
  eRenderPass m_materialPass;
  eTextureUnit ShiftTextureUnit();
  eTextureUnit m_nextTextureUnit;
  eTextureUnit m_markTextureUnit;
  iTexture* m_textures[eTU_COUNT];


  /*
   * Matrix handling 
   */

  void UpdateModelViewMatrix();
  void UpdateViewProjectionMatrix();
  void UpdateModelViewProjectionMatrix();
  void UpdateModelMatrixInv();
  void UpdateViewMatrixInv();
  void UpdateProjectionMatrixInv();
  void UpdateModelViewMatrixInv();
  void UpdateViewProjectionMatrixInv();
  void UpdateModelViewProjectionMatrixInv();
  void UpdateShadowMapViewProjectionMatrix();


  uint8_t m_colorWrite;
  bool m_depthWrite;
  bool m_depthTest;

  bool m_blending;
  eBlendFactor m_srcFactorColor;
  eBlendFactor m_srcFactorAlpha;
  eBlendFactor m_dstFactorColor;
  eBlendFactor m_dstFactorAlpha;

  Matrix4f m_modelMatrix;
  Matrix4f m_viewMatrix;
  Matrix4f m_projectionMatrix;


  Matrix4f m_modelViewMatrix;
  Matrix4f m_viewProjectionMatrix;
  Matrix4f m_modelViewProjectionMatrix;

  Matrix4f m_modelMatrixInv;
  Matrix4f m_viewMatrixInv;
  Matrix4f m_projectionMatrixInv;

  Matrix4f m_modelViewMatrixInv;
  Matrix4f m_viewProjectionMatrixInv;
  Matrix4f m_modelViewProjectionMatrixInv;

  bool m_modelViewMatrixDirty;
  bool m_viewProjectionMatrixDirty;
  bool m_modelViewProjectionMatrixDirty;

  bool m_modelMatrixInvDirty;
  bool m_viewMatrixInvDirty;
  bool m_projectionMatrixInvDirty;

  bool m_modelViewMatrixInvDirty;
  bool m_viewProjectionMatrixInvDirty;
  bool m_modelViewProjectionMatrixInvDirty;

  Size m_shadowMapMatrixCount;
  Matrix4f m_shadowMapViewMatrices[6];
  Matrix4f m_shadowMapProjectionMatrices[6];
  Matrix4f m_shadowMapViewProjectionMatrices[6];
  bool m_shadowMapViewProjectionMatrixDirty;


  struct PointLightShadowData
  {
    iLight* Light;
    iTextureCube* Color;
    iTextureCube* Depth;
    Vector3f Mapping;
  };
  std::map<const iLight*, PointLightShadowData> m_pointLightShadowData;

  struct DirectionalLightShadowData
  {
    iLight* Light;
    iTexture2DArray* Color;
    iTexture2DArray* Depth;
    Vector4f LayersBias;
    Matrix4f Matrices[3];
  };
  std::map<const iLight*, DirectionalLightShadowData> m_directionalLightShadowData;


  /** 
   * \name Fullscreen Rendering
   * @{
   */
  GL4Program* FullscreenBlitProgram();
  GL4Program* FullscreenBlitMSProgram();
  GL4Program* m_fullscreenBlitProgram;
  GL4Program* m_fullscreenBlitMSProgram;
  iRenderMesh* FullscreenBlitRenderMesh();
  iRenderMesh* m_fullscreenBlitRenderMesh;

  GL4Program* FullscreenBlitArrayProgram();
  GL4Program* m_fullscreenBlitArrayProgram;


  GL4Program* FullscreenBlitCubeProgram();
  GL4Program* m_fullscreenBlitCubeProgram;
  iRenderMesh* FullscreenBlitCubeRenderMesh(int layer);
  iRenderMesh* m_fullscreenBlitCubePosXRenderMesh;
  iRenderMesh* m_fullscreenBlitCubePosYRenderMesh;
  iRenderMesh* m_fullscreenBlitCubePosZRenderMesh;
  iRenderMesh* m_fullscreenBlitCubeNegXRenderMesh;
  iRenderMesh* m_fullscreenBlitCubeNegYRenderMesh;
  iRenderMesh* m_fullscreenBlitCubeNegZRenderMesh;

#if _DEBUG
  Size m_numDrawCalls;
  Size m_numTriangles;
  Size m_numShaderStatesChanges;
#endif

  /**
   * @}
   */
};


}