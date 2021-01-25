

#pragma once


#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>
#include <spcCore/types.hh>
#include <spcCore/graphics/erenderpass.hh>
#include <spcCore/graphics/idevice.hh>
#include <spcCore/graphics/itexture.hh>
#include <spcCore/graphics/material/ematerialattributetype.hh>
#include <spcCore/graphics/material/imaterial.hh>
#include <spcCore/math/color4f.hh>
#include <spcCore/math/matrix.hh>
#include <spcCore/math/vector.hh>
#include <string>

namespace spc
{

struct iShader;
struct iShaderAttribute;
struct iTexture;


SPC_CLASS()
class SPC_CORE_API Material : public SPC_SUPER(iMaterial)
{
  friend class MaterialInstance;
  SPC_CLASS_GEN_OBJECT;
public:
  static const Size UndefinedIndex = ~0x00;

  Material();
  virtual ~Material();

  bool Bind(iDevice * device, eRenderPass pass) override;


  void SetShader(eRenderPass pass, iShader * shader);
  iShader* GetShader(eRenderPass pass);
  const iShader* GetShader(eRenderPass pass) const;

  void RegisterAttribute(const std::string & attributeName);
  UInt16 GetNumberOfAttributes() const;

  std::vector<std::string> GetAttributeNames() const;

  Size IndexOf(const std::string & attributeName) override;

  void Set(Size idx, float value) override;
  void Set(Size idx, const Vector2f & v) override;
  void Set(Size idx, const Vector3f & v) override;
  void Set(Size idx, const Vector4f & v) override;
  void Set(Size idx, const Color4f & v) override;
  void Set(Size idx, int value) override;
  void Set(Size idx, const Matrix3f & m) override;
  void Set(Size idx, const Matrix4f & m) override;
  void Set(Size idx, iTexture * texture) override;


private:
  bool BindShader(iDevice * device, eRenderPass pass);
  void BindAttribute(iDevice * device, eRenderPass pass, Size idx);
  void BindAttribute(iDevice * device, eRenderPass pass, Size idx, float* floats, int* ints, iTexture * texture);
  void UpdateShaderAttributes(eRenderPass pass);

  struct Attribute
  {
    iShaderAttribute* Attributes[eRP_COUNT];
    std::string Name;
    eMaterialAttributeType Type;
    float Floats[16];
    int Ints[4];
    iTexture* Texture;
  };


  iShader* m_shader[eRP_COUNT];
  std::vector<Attribute> m_attributes;

};


}