
#include "csMaterial.hh"
#include <csCore/graphics/shading/iShader.hh>
#include <csCore/graphics/shading/iShaderAttribute.hh>
#include <bit>

namespace cryo
{
std::map<iMaterial *, std::string> s_material_names;


csMaterial::csMaterial()
    : iMaterial()
{
  CS_CLASS_GEN_CONSTR;
  for (auto &item: m_shader)
  {
    item = nullptr;
  }
}


csMaterial::~csMaterial()
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

void csMaterial::SetFillMode(eFillMode fillMode)
{
  m_fillMode = fillMode;
}

eFillMode csMaterial::GetFillMode() const
{
  return m_fillMode;
}

void csMaterial::SetRenderQueue(eRenderQueue queue)
{
  m_queue = queue;
}

eRenderQueue csMaterial::GetRenderQueue() const
{
  return m_queue;
}

void csMaterial::SetBlending(bool blending)
{
  m_blending = blending;
}

bool csMaterial::IsBlending() const
{
  return m_blending;
}

void csMaterial::SetBlendFactor(eBlendFactor srcFactor, eBlendFactor dstFactor)
{
  m_srcFactorColor = srcFactor;
  m_srcFactorAlpha = srcFactor;
  m_dstFactorColor = dstFactor;
  m_dstFactorAlpha = dstFactor;
}

void csMaterial::SetBlendFactor(eBlendFactor srcFactorColor,
                                eBlendFactor srcFactorAlpha,
                                eBlendFactor dstFactorColor,
                                eBlendFactor dstFactorAlpha)
{
  m_srcFactorColor = srcFactorColor;
  m_srcFactorAlpha = srcFactorAlpha;
  m_dstFactorColor = dstFactorColor;
  m_dstFactorAlpha = dstFactorAlpha;
}

eBlendFactor csMaterial::GetBlendFactorSrcColor() const
{
  return m_srcFactorColor;
}

eBlendFactor csMaterial::GetBlendFactorSrcAlpha() const
{
  return m_srcFactorAlpha;
}


eBlendFactor csMaterial::GetBlendFactorDstColor() const
{
  return m_dstFactorColor;
}

eBlendFactor csMaterial::GetBlendFactorDstAlpha() const
{
  return m_dstFactorAlpha;
}

bool csMaterial::IsDepthWrite() const
{
  return m_depthWrite;
}

void csMaterial::SetDepthWrite(bool depthWrite)
{
  m_depthWrite = depthWrite;
}

bool csMaterial::IsDepthTest() const
{
  return m_depthTest;
}

void csMaterial::SetDepthTest(bool depthTest)
{
  m_depthTest = depthTest;
}

void csMaterial::SetDepthFunc(eCompareFunc depthFun)
{
  m_depthFunc = depthFun;
}

eCompareFunc csMaterial::GetDepthFunc() const
{
  return m_depthFunc;
}


void csMaterial::SetShadingMode(eShadingMode shadingMode)
{
  m_shadingMode = shadingMode;
}

eShadingMode csMaterial::GetShadingMode() const
{
  return m_shadingMode;
}

bool csMaterial::Bind(iDevice *device, eRenderPass pass)
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

bool csMaterial::BindShader(iDevice *device, eRenderPass pass) const
{
  iShader *shader = m_shader[pass];
  if (!shader)
  {
    return false;
  }

  device->SetShader(shader);
  return true;
}

void csMaterial::BindBlending(iDevice *device) const
{
  device->SetBlending(m_blending);
//  if (m_blending)
  {
    device->SetBlendFactor(m_srcFactorColor, m_srcFactorAlpha, m_dstFactorColor, m_dstFactorAlpha);
  }
}

void csMaterial::BindDepthMode(iDevice *device) const
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

void csMaterial::BindFillMode(iDevice *device) const
{
  device->SetFillMode(m_fillMode);
}

bool csMaterial::BindAttribute(iDevice *device, eRenderPass pass, size_t idx) const
{
  const Attribute &attr = m_attributes[idx];
  return BindAttribute(device, pass, idx, attr.Floats, attr.Ints, attr.Texture);
}

bool csMaterial::BindAttribute(iDevice *device,
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
        shaderAttribute->Bind(*std::bit_cast<const csVector2f *>(floats.data()));
        break;
      case eMAT_Vec3:
        shaderAttribute->Bind(*std::bit_cast<const csVector3f *>(floats.data()));
        break;
      case eMAT_Vec4:
        shaderAttribute->Bind(*std::bit_cast<const csVector4f *>(floats.data()));
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
        shaderAttribute->Bind(*std::bit_cast<const csMatrix3f *>(floats.data()));
        break;
      case eMAT_Matrix4:
        shaderAttribute->Bind(*std::bit_cast<const csMatrix4f *>(floats.data()));
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

bool csMaterial::BindTexture(iDevice *device, iShaderAttribute *attribute, iTexture *texture)
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

void csMaterial::SetShader(eRenderPass pass, iShader *shader)
{
  CS_SET(m_shader[pass], shader);

  UpdateShaderAttributes(pass);
}

iShader *csMaterial::GetShader(eRenderPass pass)
{
  return m_shader[pass];
}


const iShader *csMaterial::GetShader(eRenderPass pass) const
{
  return m_shader[pass];
}


void csMaterial::RegisterAttribute(const std::string &attributeName, eMaterialAttributeType attributeType)
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


void csMaterial::UpdateShaderAttributes(eRenderPass pass)
{
  for (Attribute &attr: m_attributes)
  {
    attr.Attributes[pass] = m_shader[pass] ? m_shader[pass]->GetShaderAttribute(attr.Name) : nullptr;
  }
}


std::vector<std::string> csMaterial::GetAttributeNames() const
{
  std::vector<std::string> names;
  for (const Attribute &attr: m_attributes)
  {
    names.push_back(attr.Name);
  }
  return names;
}

Size csMaterial::IndexOf(const std::string &attributeName)
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

void csMaterial::Set(Size idx, float value)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute &attr = m_attributes[idx];
  attr.Type = eMAT_Float;
  attr.Floats[0] = value;
}


void csMaterial::Set(Size idx, const csVector2f &v)
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


void csMaterial::Set(Size idx, const csVector3f &v)
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

void csMaterial::Set(Size idx, const csVector4f &v)
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

void csMaterial::Set(Size idx, const csColor4f &v)
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


void csMaterial::Set(Size idx, int value)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute &attr = m_attributes[idx];
  attr.Type = eMAT_Int;
  attr.Ints[0] = value;
}


void csMaterial::Set(Size idx, const csMatrix3f &m)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute &attr = m_attributes[idx];
  attr.Type = eMAT_Matrix3;
  memcpy(attr.Floats.data(), &m, sizeof(float) * 9);
}

void csMaterial::Set(Size idx, const csMatrix4f &m)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute &attr = m_attributes[idx];
  attr.Type = eMAT_Matrix4;
  memcpy(attr.Floats.data(), &m, sizeof(float) * 16);
}

void csMaterial::Set(Size idx, iTexture *texture)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute &attr = m_attributes[idx];
  attr.Type = eMAT_Texture;
  CS_SET(attr.Texture, texture);

}
void csMaterial::Debug(Size idx)
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