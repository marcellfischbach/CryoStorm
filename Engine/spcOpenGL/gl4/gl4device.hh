
#pragma once

#include <spcOpenGL/openglexport.hh>
#include <spcCore/class.hh>
#include <spcCore/graphics/idevice.hh>
#include <spcCore/graphics/etextureunit.hh>
#include <map>

namespace spc::opengl
{

class GL4Program;

SPC_CLASS()
class SPC_OGL_API GL4Device : public SPC_SUPER(iDevice)
{
  SPC_CLASS_GEN_OBJECT;
public:
  GL4Device();
  virtual ~GL4Device();

  bool Initialize();

  void SetViewport(Int16 x, Int16 y, UInt16 width, UInt16 height) override;
  void Clear(bool clearColor, const Color4f & color, bool clearDepth, float depth, bool clearStencil, UInt8 stencil) override;

  void SetModelMatrix(const Matrix4f & modelMatrix) override;
  void SetViewMatrix(const Matrix4f & viewMatrix) override;
  void SetProjectionMatrix(const Matrix4f & projectionwMatrix) override;

  void SetModelMatrix(const Matrix4f& modelMatrix, const Matrix4f& modelMatrixInv) override;
  void SetViewMatrix(const Matrix4f& viewMatrix, const Matrix4f& viewMatrixInv) override;
  void SetProjectionMatrix(const Matrix4f& projectionwMatrix, const Matrix4f &projectionMatrixInv) override;

  void SetShadowMapViewMatrices(const Matrix4f * viewMatrices, Size numMatrices) override;
  void SetShadowMapProjectionMatrices(const Matrix4f * projectionMatrices, Size numMatrices) override;


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
  SPC_NODISCARD Size GetNumberOfDrawCalls() const override;
  SPC_NODISCARD Size GetNumberOfTriangles () const override;
#endif


private:
  iRenderTarget* m_renderTarget;
  iShader* m_shader;
  eTextureUnit ShiftTextureUnit();

  eTextureUnit m_nextTextureUnit;


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
  GL4Program* m_fullscreenBlitProgram;
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
#endif

  /**
   * @}
   */
};


}