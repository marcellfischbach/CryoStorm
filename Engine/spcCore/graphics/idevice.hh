#pragma once


#include <spcCore/coreexport.hh>
#include <spcCore/types.hh>
#include <spcCore/math/color4f.hh>
#include <spcCore/math/matrix.hh>
#include <spcCore/math/vector.hh>
#include <spcCore/class.hh>
#include <spcCore/graphics/etextureunit.hh>
#include <spcCore/graphics/itexture.hh>
#include <spcCore/graphics/itexture2d.hh>


namespace spc
{

struct iShader;

SPC_CLASS()
struct SPC_CORE_API iDevice : public SPC_SUPER(iObject)
{
  SPC_CLASS_GEN;

  virtual ~iDevice(){}

  virtual void SetViewport(Int16 x, Int16 y, UInt16 width, UInt16 height) = 0;
  virtual void Clear(bool clearColor, const Color4f& color, bool clearDepth, float depth, bool clearStencil, UInt8 stencil) = 0;

  virtual void SetModelMatrix(const Matrix4f& modelMatrix) = 0;
  virtual void SetViewMatrix(const Matrix4f& viewMatrix) = 0;
  virtual void SetProjectionMatrix(const Matrix4f& projectionwMatrix) = 0;

  virtual void SetModelMatrix(const Matrix4f& modelMatrix, const Matrix4f& modelMatrixInv) = 0;
  virtual void SetViewMatrix(const Matrix4f& viewMatrix, const Matrix4f& viewMatrixInv) = 0;
  virtual void SetProjectionMatrix(const Matrix4f& projectionwMatrix, const Matrix4f &projectionMatrixInv) = 0;

  virtual void SetShader(iShader* shader) = 0;

  virtual iTexture2D* CreateTexture(const iTexture2D::Descriptor& descriptor) = 0;
  virtual void ResetTextures() = 0;
  virtual eTextureUnit BindTexture(iTexture* texture) = 0;
};

}