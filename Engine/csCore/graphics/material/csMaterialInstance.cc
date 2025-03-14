
#include <csCore/graphics/material/csMaterialInstance.hh>



namespace cs
{



csMaterialInstance::csMaterialInstance()
  : iMaterial()
  , m_material(nullptr)
{

}


csMaterialInstance::~csMaterialInstance()
{

}

bool csMaterialInstance::Bind(iDevice* device, eRenderPass pass)
{
  if (!m_material)
  {
    return false;
  }
  if (!m_material->BindShader(device, pass))
  {
    return false;
  }

  m_material->BindBlending(device);
  m_material->BindDepthMode(device);
  m_material->BindFillMode(device);

  device->ResetTextures();
  for (Size i = 0, in = m_attributes.size(); i < in; i++)
  {
    Attribute& attribute = m_attributes[i];
    if (attribute.Override)
    {
      m_material->BindAttribute(device, pass, (uint16_t)i, attribute.Floats, attribute.Ints, attribute.Texture);
    }
    else
    {
      m_material->BindAttribute(device, pass, (uint16_t)i);
    }
  }
  device->MarkTexture();
  return true;
}

void csMaterialInstance::SetMaterial(csMaterial *material)
{
  m_material = material;
  RebuildAttributes();
}

csMaterial *csMaterialInstance::GetMaterial()
{
  return m_material;
}

const csMaterial *csMaterialInstance::GetMaterial() const
{
  return m_material;
}


Size csMaterialInstance::IndexOf(const std::string& attributeName)
{
  return m_material ? m_material->IndexOf(attributeName) : csMaterial::UndefinedIndex;
}


void csMaterialInstance::SetFloat(size_t idx, float value)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute& attr = m_attributes[idx];
  attr.Override = true;
  attr.Floats[0] = value;
}


void csMaterialInstance::SetVector2f(size_t idx, const cs::csVector2f &v)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute& attr = m_attributes[idx];
  attr.Override = true;
  attr.Floats[0] = v.x;
  attr.Floats[1] = v.y;
}


void csMaterialInstance::SetVector3f(size_t idx, const cs::csVector3f &v)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute& attr = m_attributes[idx];
  attr.Override = true;
  attr.Floats[0] = v.x;
  attr.Floats[1] = v.y;
  attr.Floats[2] = v.z;
}

void csMaterialInstance::SetVector4f(size_t idx, const cs::csVector4f &v)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute& attr = m_attributes[idx];
  attr.Override = true;
  attr.Floats[0] = v.x;
  attr.Floats[1] = v.y;
  attr.Floats[2] = v.z;
  attr.Floats[3] = v.w;
}


void csMaterialInstance::SetColor4f(size_t idx, const cs::csColor4f &v)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute& attr = m_attributes[idx];
  attr.Override = true;
  attr.Floats[0] = v.r;
  attr.Floats[1] = v.g;
  attr.Floats[2] = v.b;
  attr.Floats[3] = v.a;
}

void csMaterialInstance::SetInt(size_t idx, int value)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute& attr = m_attributes[idx];
  attr.Override = true;
  attr.Ints[0] = value;
}


void csMaterialInstance::SetMatrix3f(size_t idx, const cs::csMatrix3f &m)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute& attr = m_attributes[idx];
  attr.Override = true;
  memcpy(attr.Floats.data(), &m, sizeof(float) * 9);
}

void csMaterialInstance::SetMatrix4f(size_t idx, const cs::csMatrix4f &m)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute& attr = m_attributes[idx];
  attr.Override = true;
  memcpy(attr.Floats.data(), &m, sizeof(float) * 16);
}

void csMaterialInstance::SetTexture(size_t idx, cs::iTexture *texture)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute& attr = m_attributes[idx];
  attr.Override = true;
  attr.Texture = texture;
}

bool csMaterialInstance::IsOverridden(Size idx) const
{
  if (idx >= m_attributes.size())
  {
    return false;
  }
  const Attribute& attr = m_attributes[idx];
  return attr.Override;
}

void csMaterialInstance::SetOverride(Size idx, bool _override)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  m_attributes[idx].Override = _override;
}

void csMaterialInstance::RebuildAttributes()
{
  m_attributes.clear();
  if (m_material)
  {
    std::vector<std::string> names = m_material->GetAttributeNames();
    for (auto name : names)
    {
      Attribute attribute;
      attribute.Override = false;
      attribute.Texture = nullptr;
      m_attributes.push_back(attribute);
    }
  }
}


const iShader *csMaterialInstance::GetShader(eRenderPass pass) const
{
  return m_material ? m_material->GetShader(pass) : nullptr;
}

eFillMode csMaterialInstance::GetFillMode() const
{
  return m_material ? m_material->GetFillMode() : eFillMode::Fill;
}

eRenderQueue csMaterialInstance::GetRenderQueue() const
{
  return m_material ? m_material->GetRenderQueue() : eRenderQueue::Default;
}

eShadingMode csMaterialInstance::GetShadingMode() const
{
  return m_material ? m_material->GetShadingMode() : eShadingMode::Shaded;
}

}