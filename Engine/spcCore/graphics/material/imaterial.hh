
#pragma once


#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>
#include <spcCore/math/color4f.hh>
#include <spcCore/math/vector.hh>
#include <spcCore/math/matrix.hh>
#include <spcCore/graphics/erenderqueue.hh>
#include <spcCore/graphics/itexture.hh>
#include <string>


namespace spc
{

SPC_CLASS()
struct SPC_CORE_API iMaterial : public SPC_SUPER(iObject)
{
  SPC_CLASS_GEN;

  ~iMaterial() override = default;

  SPC_NODISCARD virtual eRenderQueue GetRenderQueue() const = 0;

  virtual bool Bind(iDevice * device, eRenderPass pass) = 0;

  virtual Size IndexOf(const std::string & attributeName) = 0;

  virtual void Set(Size idx, float value) = 0;
  virtual void Set(Size idx, const Vector2f & v) = 0;
  virtual void Set(Size idx, const Vector3f & v) = 0;
  virtual void Set(Size idx, const Vector4f & v) = 0;
  virtual void Set(Size idx, const Color4f & v) = 0;
  virtual void Set(Size idx, int value) = 0;
  virtual void Set(Size idx, const Matrix3f & m) = 0;
  virtual void Set(Size idx, const Matrix4f & m) = 0;
  virtual void Set(Size idx, iTexture * texture) = 0;
};

}