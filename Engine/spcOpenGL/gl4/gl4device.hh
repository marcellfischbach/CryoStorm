
#pragma once

#include <spcOpenGL/openglexport.hh>
#include <spcCore/class.hh>
#include <spcCore/graphics/idevice.hh>
#include <spcCore/graphics/itexture.hh>
#include <spcCore/graphics/itexture2d.hh>
#include <spcCore/graphics/etextureunit.hh>

namespace spc
{


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

  void SetModelMatrix(const Matrix4f & modelMatrix);
  void SetViewMatrix(const Matrix4f & viewMatrix);
  void SetProjectionMatrix(const Matrix4f & projectionwMatrix);

  void SetModelMatrix(const Matrix4f& modelMatrix, const Matrix4f& modelMatrixInv);
  void SetViewMatrix(const Matrix4f& viewMatrix, const Matrix4f& viewMatrixInv);
  void SetProjectionMatrix(const Matrix4f& projectionwMatrix, const Matrix4f &projectionMatrixInv);


  void SetShader(iShader * shader) override;

  iTexture2D* CreateTexture(const iTexture2D::Descriptor & descriptor) override;
  void ResetTextures() override;
  eTextureUnit BindTexture(iTexture * texture) override;

  void BindMatrices();

private:
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
};


}