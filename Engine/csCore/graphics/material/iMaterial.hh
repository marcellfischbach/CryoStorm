
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
#include <csCore/resource/iAsset.hh>
#include <csCore/csRef.hh>
#include <string>
#include <map>
#include <csCore/graphics/eShadingMode.hh>


namespace cs
{

struct iDevice;
struct iShader;

CS_CLASS(jclass="org.cryo.core.graphics.material.IMaterial")
struct CS_CORE_API iMaterial : public CS_SUPER(iAsset)
{
  CS_CLASS_GEN;

  ~iMaterial() override = default;


  CS_FUNCTION(jenum)
  CS_NODISCARD virtual csAssetRef<const cs::iShader> GetShader (cs::eRenderPass pass) const = 0;

  CS_FUNCTION(jenum)
  CS_NODISCARD virtual cs::eFillMode GetFillMode() const = 0;
  CS_FUNCTION(jenum)
  CS_NODISCARD virtual cs::eRenderQueue GetRenderQueue() const = 0;
  CS_FUNCTION(jenum)
  CS_NODISCARD virtual cs::eShadingMode GetShadingMode () const = 0;

  CS_FUNCTION(jenum="cs::eRenderPass")
  virtual bool Bind(cs::iDevice * device, cs::eRenderPass pass) = 0;

  CS_FUNCTION()
  virtual size_t IndexOf(const std::string & attributeName) = 0;
  CS_FUNCTION()
  virtual void Debug (size_t idx) {}

  CS_FUNCTION()
  virtual void SetFloat(size_t idx, float value) = 0;
  CS_FUNCTION()
  virtual void SetVector2f(size_t idx, const cs::csVector2f & v) = 0;
  CS_FUNCTION()
  virtual void SetVector3f(size_t idx, const cs::csVector3f & v) = 0;
  CS_FUNCTION()
  virtual void SetVector4f(size_t idx, const cs::csVector4f & v) = 0;
  CS_FUNCTION()
  virtual void SetColor4f(size_t idx, const cs::csColor4f & v) = 0;
  CS_FUNCTION()
  virtual void SetInt(size_t idx, int value) = 0;
  virtual void SetMatrix3f(size_t idx, const cs::csMatrix3f & m) = 0;
  virtual void SetMatrix4f(size_t idx, const cs::csMatrix4f & m) = 0;
  CS_FUNCTION()
  virtual void SetTexture(size_t idx, csAssetRef<cs::iTexture> &texture) = 0;
};


}