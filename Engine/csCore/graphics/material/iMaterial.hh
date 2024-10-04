
#pragma once



#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/math/csColor4f.hh>
#include <csCore/math/csVector.hh>
#include <csCore/math/csMatrix.hh>
#include <csCore/graphics/eFillMode.hh>
#include <csCore/graphics/eRenderPass.hh>
#include <csCore/graphics/eRenderQueue.hh>
#include <csCore/graphics/iTexture.hh>
#include <string>
#include <map>
#include <csCore/graphics/eShadingMode.hh>


namespace cryo
{

struct iDevice;
struct iShader;

CS_CLASS()
struct CS_CORE_API iMaterial : public CS_SUPER(iObject)
{
  CS_CLASS_GEN;

  ~iMaterial() override = default;

  CS_NODISCARD virtual const iShader *GetShader (eRenderPass pass) const = 0;

  CS_NODISCARD virtual eFillMode GetFillMode() const = 0;
  CS_NODISCARD virtual eRenderQueue GetRenderQueue() const = 0;
  CS_NODISCARD virtual eShadingMode GetShadingMode () const = 0;

  virtual bool Bind(iDevice * device, eRenderPass pass) = 0;

  virtual Size IndexOf(const std::string & attributeName) = 0;
  virtual void Debug (Size idx) {}

  virtual void Set(Size idx, float value) = 0;
  virtual void Set(Size idx, const csVector2f & v) = 0;
  virtual void Set(Size idx, const csVector3f & v) = 0;
  virtual void Set(Size idx, const csVector4f & v) = 0;
  virtual void Set(Size idx, const csColor4f & v) = 0;
  virtual void Set(Size idx, int value) = 0;
  virtual void Set(Size idx, const csMatrix3f & m) = 0;
  virtual void Set(Size idx, const csMatrix4f & m) = 0;
  virtual void Set(Size idx, iTexture * texture) = 0;
};


CS_CORE_API extern std::map<iMaterial*, std::string> s_material_names;

}