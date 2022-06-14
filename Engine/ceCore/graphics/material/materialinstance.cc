
#include <ceCore/graphics/material/materialinstance.hh>



namespace ce
{



MaterialInstance::MaterialInstance()
  : iMaterial()
  , m_material(nullptr)
{
  CE_CLASS_GEN_CONSTR;
}


MaterialInstance::~MaterialInstance()
{

}

bool MaterialInstance::Bind(iDevice* device, eRenderPass pass)
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

void MaterialInstance::SetMaterial(Material* material)
{
  CE_SET(m_material, material);
  RebuildAttributes();
}

Material* MaterialInstance::GetMaterial()
{
  return m_material;
}

const Material* MaterialInstance::GetMaterial() const
{
  return m_material;
}


Size MaterialInstance::IndexOf(const std::string& attributeName)
{
  return m_material ? m_material->IndexOf(attributeName) : Material::UndefinedIndex;
}



void MaterialInstance::Set(Size idx, float value)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute& attr = m_attributes[idx];
  attr.Override = true;
  attr.Floats[0] = value;
}


void MaterialInstance::Set(Size idx, const Vector2f& v)
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


void MaterialInstance::Set(Size idx, const Vector3f& v)
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

void MaterialInstance::Set(Size idx, const Vector4f& v)
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


void MaterialInstance::Set(Size idx, const Color4f& v)
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

void MaterialInstance::Set(Size idx, int value)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute& attr = m_attributes[idx];
  attr.Override = true;
  attr.Ints[0] = value;
}


void MaterialInstance::Set(Size idx, const Matrix3f& m)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute& attr = m_attributes[idx];
  attr.Override = true;
  memcpy(attr.Floats.data(), &m, sizeof(float) * 9);
}

void MaterialInstance::Set(Size idx, const Matrix4f& m)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute& attr = m_attributes[idx];
  attr.Override = true;
  memcpy(attr.Floats.data(), &m, sizeof(float) * 16);
}

void MaterialInstance::Set(Size idx, iTexture* texture)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute& attr = m_attributes[idx];
  attr.Override = true;
  CE_SET(attr.Texture, texture);

}

bool MaterialInstance::IsOverridden(Size idx) const
{
  if (idx >= m_attributes.size())
  {
    return false;
  }
  const Attribute& attr = m_attributes[idx];
  return attr.Override;
}

void MaterialInstance::SetOverride(Size idx, bool _override)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  m_attributes[idx].Override = _override;
}

void MaterialInstance::RebuildAttributes()
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


const iShader *MaterialInstance::GetShader(eRenderPass pass) const
{
  return m_material ? m_material->GetShader(pass) : nullptr;
}

eFillMode MaterialInstance::GetFillMode() const
{
  return m_material ? m_material->GetFillMode() : eFillMode::Fill;
}

eRenderQueue MaterialInstance::GetRenderQueue() const
{
  return m_material ? m_material->GetRenderQueue() : eRenderQueue::Default;
}

eShadingMode MaterialInstance::GetShadingMode() const
{
  return m_material ? m_material->GetShadingMode() : eShadingMode::Shaded;
}

}