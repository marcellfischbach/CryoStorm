
#include <spcCore/graphics/material/materialinstance.hh>
#include <spcCore/graphics/material/material.hh>



namespace spc
{



MaterialInstance::MaterialInstance()
  : iObject()
  , m_material(nullptr)
{
  SPC_CLASS_GEN_CONSTR;
}


MaterialInstance::~MaterialInstance()
{

}

void MaterialInstance::SetMaterial(Material* material)
{
  SPC_SET(m_material, material);
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


UInt16 MaterialInstance::IndexOf(const std::string& attributeName)
{
  return m_material ? m_material->IndexOf(attributeName) : Material::UndefinedIndex;
}



void MaterialInstance::Set(UInt16 idx, float value)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute& attr = m_attributes[idx];
  attr.Override = true;
  attr.Floats[0] = value;
}


void MaterialInstance::Set(UInt16 idx, const Vector2f& v)
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


void MaterialInstance::Set(UInt16 idx, const Vector3f& v)
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

void MaterialInstance::Set(UInt16 idx, const Vector4f& v)
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


void MaterialInstance::Set(UInt16 idx, int value)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute& attr = m_attributes[idx];
  attr.Override = true;
  attr.Ints[0] = value;
}


void MaterialInstance::Set(UInt16 idx, const Matrix3f& m)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute& attr = m_attributes[idx];
  attr.Override = true;
  memcpy(attr.Floats, &m, sizeof(float) * 9);
}

void MaterialInstance::Set(UInt16 idx, const Matrix4f& m)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute& attr = m_attributes[idx];
  attr.Override = true;
  memcpy(attr.Floats, &m, sizeof(float) * 16);
}

void MaterialInstance::Set(UInt16 idx, iTexture* texture)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute& attr = m_attributes[idx];
  attr.Override = true;
  SPC_SET(attr.Texture, texture);

}

bool MaterialInstance::IsOverridden(UInt16 idx) const
{
  if (idx >= m_attributes.size())
  {
    return false;
  }
  const Attribute& attr = m_attributes[idx];
  return attr.Override;
}

void MaterialInstance::RebuildAttributes()
{
  m_attributes.clear();
  if (m_material)
  {
    std::vector<std::string> names = m_material->GetAttributeNames();

  }
}

}