

#pragma once


#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/types.hh>
#include <ceCore/graphics/erenderpass.hh>
#include <ceCore/graphics/itexture.hh>
#include <ceCore/graphics/material/ematerialattributetype.hh>
#include <ceCore/graphics/material/material.hh>
#include <ceCore/graphics/material/imaterial.hh>
#include <ceCore/math/matrix.hh>
#include <ceCore/math/vector.hh>
#include <string>

namespace ce
{

class Material;

CE_CLASS()
class CE_CORE_API MaterialInstance : public CE_SUPER(iMaterial)
{
  CE_CLASS_GEN_OBJECT;
public:

  MaterialInstance();
  virtual ~MaterialInstance();

  const iShader *GetShader(eRenderPass pass) const override;

  eFillMode GetFillMode () const override;
  eRenderQueue GetRenderQueue() const override;
  eShadingMode GetShadingMode() const override;

  bool Bind(iDevice * device, eRenderPass pass) override;


  void SetMaterial(Material * material);
  Material* GetMaterial();
  const Material* GetMaterial() const;

  Size IndexOf(const std::string & attributeName) override;

  void Set(Size idx, float value) override;
  void Set(Size idx, const Vector2f & v) override;
  void Set(Size idx, const Vector3f & v) override;
  void Set(Size idx, const Vector4f & v) override;
  void Set(Size idx, const Color4f& v) override;
  void Set(Size idx, int value) override;
  void Set(Size idx, const Matrix3f & m) override;
  void Set(Size idx, const Matrix4f & m) override;
  void Set(Size idx, iTexture * texture) override;

  bool IsOverridden(Size idx) const;
  void SetOverride(Size idx, bool _override);
private:

  void RebuildAttributes();

  struct Attribute
  {
    bool Override;
    std::array<float, 16> Floats;
    std::array<int, 4> Ints;
    iTexture* Texture;
  };


  Material* m_material;
  std::vector<Attribute> m_attributes;
};


}