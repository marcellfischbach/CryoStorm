

#pragma once


#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>
#include <spcCore/types.hh>
#include <spcCore/graphics/erenderpass.hh>
#include <spcCore/graphics/itexture.hh>
#include <spcCore/graphics/material/ematerialattributetype.hh>
#include <spcCore/graphics/material/material.hh>
#include <spcCore/graphics/material/imaterial.hh>
#include <spcCore/math/matrix.hh>
#include <spcCore/math/vector.hh>
#include <string>

namespace spc
{

class Material;

SPC_CLASS()
class SPC_CORE_API MaterialInstance : public SPC_SUPER(iMaterial)
{
  SPC_CLASS_GEN_OBJECT;
public:

  MaterialInstance();
  virtual ~MaterialInstance();

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
    float Floats[16];
    int Ints[4];
    iTexture* Texture;
  };


  Material* m_material;
  std::vector<Attribute> m_attributes;

};


}