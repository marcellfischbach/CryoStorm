
#pragma once



#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/math/color4f.hh>
#include <ceCore/math/vector.hh>
#include <ceCore/math/matrix.hh>
#include <ceCore/graphics/efillmode.hh>
#include <ceCore/graphics/erenderpass.hh>
#include <ceCore/graphics/erenderqueue.hh>
#include <ceCore/graphics/itexture.hh>
#include <string>
#include <ceCore/graphics/eshadingmode.hh>


namespace ce
{

struct iDevice;
struct iShader;

CE_CLASS()
struct CE_CORE_API iMaterial : public CE_SUPER(iObject)
{
  CE_CLASS_GEN;

  ~iMaterial() override = default;

  CE_NODISCARD virtual const iShader *GetShader (eRenderPass pass) const = 0;

  CE_NODISCARD virtual eFillMode GetFillMode() const = 0;
  CE_NODISCARD virtual eRenderQueue GetRenderQueue() const = 0;
  CE_NODISCARD virtual eShadingMode GetShadingMode () const = 0;

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