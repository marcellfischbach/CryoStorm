
#include <ceCore/graphics/material/material.hh>
#include <ceCore/graphics/shading/ishader.hh>
#include <ceCore/graphics/shading/ishaderattribute.hh>
#include <bit>

namespace cryo
{
std::map<iMaterial *, std::string> s_material_names;


Material::Material()
    : iMaterial()
{
  CS_CLASS_GEN_CONSTR;
  for (auto &item: m_shader)
  {
    item = nullptr;
  }
}


Material::~Material()
{
  for (Size i = 0; i < eRP_COUNT; i++)
  {
    CS_RELEASE(m_shader[i]);
    m_shader[i] = nullptr;
  }

  for (auto attribute: m_attributes)
  {
    CS_RELEASE(attribute.Texture);
  }
}

void Material::SetFillMode(eFillMode fillMode)
{
  m_fillMode = fillMode;
}

eFillMode Material::GetFillMode() const
{
  return m_fillMode;
}

void Material::SetRenderQueue(eRenderQueue queue)
{
  m_queue = queue;
}

eRenderQueue Material::GetRenderQueue() const
{
  return m_queue;
}

void Material::SetBlending(bool blending)
{
  m_blending = blending;
}

bool Material::IsBlending() const
{
  return m_blending;
}

void Material::SetBlendFactor(eBlendFactor srcFactor, eBlendFactor dstFactor)
{
  m_srcFactorColor = srcFactor;
  m_srcFactorAlpha = srcFactor;
  m_dstFactorColor = dstFactor;
  m_dstFactorAlpha = dstFactor;
}

void Material::SetBlendFactor(eBlendFactor srcFactorColor,
                              eBlendFactor srcFactorAlpha,
                              eBlendFactor dstFactorColor,
                              eBlendFactor dstFactorAlpha)
{
  m_srcFactorColor = srcFactorColor;
  m_srcFactorAlpha = srcFactorAlpha;
  m_dstFactorColor = dstFactorColor;
  m_dstFactorAlpha = dstFactorAlpha;
}

eBlendFactor Material::GetBlendFactorSrcColor() const
{
  return m_srcFactorColor;
}

eBlendFactor Material::GetBlendFactorSrcAlpha() const
{
  return m_srcFactorAlpha;
}


eBlendFactor Material::GetBlendFactorDstColor() const
{
  return m_dstFactorColor;
}

eBlendFactor Material::GetBlendFactorDstAlpha() const
{
  return m_dstFactorAlpha;
}

bool Material::IsDepthWrite() const
{
  return m_depthWrite;
}

void Material::SetDepthWrite(bool depthWrite)
{
  m_depthWrite = depthWrite;
}

bool Material::IsDepthTest() const
{
  return m_depthTest;
}

void Material::SetDepthTest(bool depthTest)
{
  m_depthTest = depthTest;
}

void Material::SetDepthFunc(eCompareFunc depthFun)
{
  m_depthFunc = depthFun;
}

eCompareFunc Material::GetDepthFunc() const
{
  return m_depthFunc;
}


void Material::SetShadingMode(eShadingMode shadingMode)
{
  m_shadingMode = shadingMode;
}

eShadingMode Material::GetShadingMode() const
{
  return m_shadingMode;
}

bool Material::Bind(iDevice *device, eRenderPass pass)
{
  if (!BindShader(device, pass))
  {
    return false;
  }
  BindBlending(device);
  BindDepthMode(device);
  BindFillMode(device);
  device->ResetTextures();
  for (Size i = 0, in = m_attributes.size(); i < in; ++i)
  {
    BindAttribute(device, pass, i);
  }
  device->MarkTexture();
  return true;
}

bool Material::BindShader(iDevice *device, eRenderPass pass) const
{
  iShader *shader = m_shader[pass];
  if (!shader)
  {
    return false;
  }

  device->SetShader(shader);
  return true;
}

void Material::BindBlending(iDevice *device) const
{
  device->SetBlending(m_blending);
//  if (m_blending)
  {
    device->SetBlendFactor(m_srcFactorColor, m_srcFactorAlpha, m_dstFactorColor, m_dstFactorAlpha);
  }
}

void Material::BindDepthMode(iDevice *device) const
{
  if (m_depthWrite && !m_depthTest)
  {
    device->SetDepthTest(true);
    device->SetDepthWrite(true);
    device->SetDepthFunc(eCF_Always);
  }
  else
  {
    device->SetDepthWrite(m_depthWrite);
    device->SetDepthTest(m_depthTest);
    device->SetDepthFunc(m_depthFunc);
  }
}

void Material::BindFillMode(iDevice *device) const
{
  device->SetFillMode(m_fillMode);
}

bool Material::BindAttribute(iDevice *device, eRenderPass pass, size_t idx) const
{
  const Attribute &attr = m_attributes[idx];
  return BindAttribute(device, pass, idx, attr.Floats, attr.Ints, attr.Texture);
}

bool Material::BindAttribute(iDevice *device,
                             eRenderPass pass,
                             size_t idx,
                             const std::array<float, 16> &floats,
                             const std::array<int, 4> &ints,
                             iTexture *texture) const
{

  if (const Attribute &attribute = m_attributes[idx]; iShaderAttribute *shaderAttribute = attribute.Attributes[pass])
  {
    switch (attribute.Type)
    {
      case eMAT_Float:
        shaderAttribute->Bind(floats[0]);
        break;
      case eMAT_Vec2:
        shaderAttribute->Bind(*std::bit_cast<const Vector2f *>(floats.data()));
        break;
      case eMAT_Vec3:
        shaderAttribute->Bind(*std::bit_cast<const Vector3f *>(floats.data()));
        break;
      case eMAT_Vec4:
        shaderAttribute->Bind(*std::bit_cast<const Vector4f *>(floats.data()));
        break;
      case eMAT_Int:
        shaderAttribute->Bind(ints[0]);
        break;
      case eMAT_IVec2:
      case eMAT_IVec3:
      case eMAT_IVec4:
        // TODO: Need integer based vectors
        break;
      case eMAT_Matrix3:
        shaderAttribute->Bind(*std::bit_cast<const Matrix3f *>(floats.data()));
        break;
      case eMAT_Matrix4:
        shaderAttribute->Bind(*std::bit_cast<const Matrix4f *>(floats.data()));
        break;
      case eMAT_Texture:
        if (!BindTexture(device, shaderAttribute, texture))
        {
          return false;
        }
        break;
      default:
        break;
    }
  }
  return true;
}

bool Material::BindTexture(iDevice *device, iShaderAttribute *attribute, iTexture *texture)
{
  eTextureUnit unit = device->BindTexture(texture);
//  printf ("Bind texture: %p -> %d\n", texture, unit);
  if (unit == eTU_Invalid)
  {
    return false;
  }
  attribute->Bind(unit);
  return true;
}

void Material::SetShader(eRenderPass pass, iShader *shader)
{
  CS_SET(m_shader[pass], shader);

  UpdateShaderAttributes(pass);
}

iShader *Material::GetShader(eRenderPass pass)
{
  return m_shader[pass];
}


const iShader *Material::GetShader(eRenderPass pass) const
{
  return m_shader[pass];
}


void Material::RegisterAttribute(const std::string &attributeName, eMaterialAttributeType attributeType)
{
  Attribute attribute;
  attribute.Name    = attributeName;
  attribute.Texture = nullptr;
  for (int i = 0; i < eRP_COUNT; ++i)
  {
    attribute.Attributes[i] = m_shader[i] ? m_shader[i]->GetShaderAttribute(attributeName) : nullptr;
  }
  attribute.Type = attributeType;
  m_attributes.push_back(attribute);
}


void Material::UpdateShaderAttributes(eRenderPass pass)
{
  for (Attribute &attr: m_attributes)
  {
    attr.Attributes[pass] = m_shader[pass] ? m_shader[pass]->GetShaderAttribute(attr.Name) : nullptr;
  }
}


std::vector<std::string> Material::GetAttributeNames() const
{
  std::vector<std::string> names;
  for (const Attribute &attr: m_attributes)
  {
    names.push_back(attr.Name);
  }
  return names;
}

Size Material::IndexOf(const std::string &attributeName)
{
  for (Size i = 0, in = m_attributes.size(); i < in; ++i)
  {
    if (m_attributes[i].Name == attributeName)
    {
      return i;
    }
  }
  return UndefinedIndex;
}

void Material::Set(Size idx, float value)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute &attr = m_attributes[idx];
  attr.Type = eMAT_Float;
  attr.Floats[0] = value;
}


void Material::Set(Size idx, const Vector2f &v)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute &attr = m_attributes[idx];
  attr.Type = eMAT_Vec2;
  attr.Floats[0] = v.x;
  attr.Floats[1] = v.y;
}


void Material::Set(Size idx, const Vector3f &v)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute &attr = m_attributes[idx];
  attr.Type = eMAT_Vec3;
  attr.Floats[0] = v.x;
  attr.Floats[1] = v.y;
  attr.Floats[2] = v.z;
}

void Material::Set(Size idx, const Vector4f &v)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute &attr = m_attributes[idx];
  attr.Type = eMAT_Vec4;
  attr.Floats[0] = v.x;
  attr.Floats[1] = v.y;
  attr.Floats[2] = v.z;
  attr.Floats[3] = v.w;
}

void Material::Set(Size idx, const Color4f &v)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute &attr = m_attributes[idx];
  attr.Type = eMAT_Vec4;
  attr.Floats[0] = v.r;
  attr.Floats[1] = v.g;
  attr.Floats[2] = v.b;
  attr.Floats[3] = v.a;
}


void Material::Set(Size idx, int value)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute &attr = m_attributes[idx];
  attr.Type = eMAT_Int;
  attr.Ints[0] = value;
}


void Material::Set(Size idx, const Matrix3f &m)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute &attr = m_attributes[idx];
  attr.Type = eMAT_Matrix3;
  memcpy(attr.Floats.data(), &m, sizeof(float) * 9);
}

void Material::Set(Size idx, const Matrix4f &m)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute &attr = m_attributes[idx];
  attr.Type = eMAT_Matrix4;
  memcpy(attr.Floats.data(), &m, sizeof(float) * 16);
}

void Material::Set(Size idx, iTexture *texture)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute &attr = m_attributes[idx];
  attr.Type = eMAT_Texture;
  CS_SET(attr.Texture, texture);

}
void Material::Debug(Size idx)
{
  if (idx >= m_attributes.size())
  {
    return;
  }

  Attribute &attr = m_attributes[idx];
  if (attr.Type == eMAT_Float)
  {
    printf("%.2f", attr.Floats[0]);
  }
}


}