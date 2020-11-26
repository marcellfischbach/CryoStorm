

#pragma once


#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>
#include <spcCore/types.hh>
#include <spcCore/graphics/erenderpass.hh>
#include <spcCore/graphics/itexture.hh>
#include <spcCore/graphics/material/ematerialattributetype.hh>
#include <spcCore/math/matrix.hh>
#include <spcCore/math/vector.hh>
#include <string>

namespace spc
{

struct iShader;
struct iShaderAttribute;
struct iTexture;


SPC_CLASS()
class SPC_CORE_API Material : public SPC_SUPER(iObject)
{
public:
  const UInt16 UndefinedIndex = ~0x00;

  Material();
  virtual ~Material();

  void RegisterAttribute(const std::string & attributeName);


  UInt16 IndexOf(const std::string & attributeName);

  void Set(UInt16 idx, float value);
  void Set(UInt16 idx, const Vector2f & v);
  void Set(UInt16 idx, const Vector3f & v);
  void Set(UInt16 idx, const Vector4f & v);
  void Set(UInt16 idx, int value);
  void Set(UInt16 idx, const Matrix3f & m);
  void Set(UInt16 idx, const Matrix4f & m);
  void Set(UInt16 idx, iTexture * texture);

private:



  struct Attribute
  {
    iShaderAttribute* Attributes[eRP_COUNT];
    std::string Name;
    eMaterialAttributeType Type;
    float Floats[16];
    float Ints[4];
    iTexture* Texture;
  };


  iShader* m_shader[eRP_COUNT];
  std::vector<Attribute> m_attributes;

};


}