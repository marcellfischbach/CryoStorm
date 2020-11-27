

#pragma once


#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>
#include <spcCore/types.hh>
#include <spcCore/graphics/erenderpass.hh>
#include <spcCore/graphics/itexture.hh>
#include <spcCore/graphics/material/ematerialattributetype.hh>
#include <spcCore/graphics/material/material.hh>
#include <spcCore/math/matrix.hh>
#include <spcCore/math/vector.hh>
#include <string>

namespace spc
{

class Material;

SPC_CLASS()
class SPC_CORE_API MaterialInstance : public SPC_SUPER(iObject)
{
  SPC_CLASS_GEN_OBJECT;
public:

  MaterialInstance();
  virtual ~MaterialInstance();

  bool Bind(iDevice * device, eRenderPass pass);


  void SetMaterial(Material * material);
  Material* GetMaterial();
  const Material* GetMaterial() const;

  UInt16 IndexOf(const std::string & attributeName);

  void Set(UInt16 idx, float value);
  void Set(UInt16 idx, const Vector2f & v);
  void Set(UInt16 idx, const Vector3f & v);
  void Set(UInt16 idx, const Vector4f & v);
  void Set(UInt16 idx, const Color4f& v);
  void Set(UInt16 idx, int value);
  void Set(UInt16 idx, const Matrix3f & m);
  void Set(UInt16 idx, const Matrix4f & m);
  void Set(UInt16 idx, iTexture * texture);

  bool IsOverridden(UInt16 idx) const;
  void SetOverride(UInt16 idx, bool _override);
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