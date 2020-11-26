
#include <spcCore/graphics/material/material.hh>
#include <spcCore/graphics/shading/ishader.hh>
#include <spcCore/graphics/shading/ishaderattribute.hh>


namespace spc
{

Material::Material()
{

}


Material::~Material()
{

}


void Material::RegisterAttribute(const std::string& attributeName)
{
  Attribute attribute;
  attribute.Name = attributeName;
  for (int i = 0; i < eRP_COUNT; ++i)
  {
    attribute.Attributes[i] = m_shader[i] ? m_shader[i]->GetShaderAttribute(attributeName) : nullptr;
  }
  attribute.Type = eMAT_Undefined;
}


UInt16 Material::IndexOf(const std::string& attributeName)
{
  for (UInt16 i = 0, in = m_attributes.size(); i < in; ++i)
  {
    if (m_attributes[i].Name == attributeName)
    {
      return i;
    }
  }
  return UndefinedIndex;
}

void Material::Set(UInt16 idx, float value)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute& attr = m_attributes[idx];
  attr.Type = eMAT_Float;
  attr.Floats[0] = value;
}


void Material::Set(UInt16 idx, const Vector2f& v)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute& attr = m_attributes[idx];
  attr.Type = eMAT_Vec2;
  attr.Floats[0] = v.x;
  attr.Floats[1] = v.y;
}


void Material::Set(UInt16 idx, const Vector3f& v)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute& attr = m_attributes[idx];
  attr.Type = eMAT_Vec3;
  attr.Floats[0] = v.x;
  attr.Floats[1] = v.y;
  attr.Floats[2] = v.z;
}

void Material::Set(UInt16 idx, const Vector4f& v)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute& attr = m_attributes[idx];
  attr.Type = eMAT_Vec4;
  attr.Floats[0] = v.x;
  attr.Floats[1] = v.y;
  attr.Floats[2] = v.z;
  attr.Floats[3] = v.w;
}


void Material::Set(UInt16 idx, int value)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute& attr = m_attributes[idx];
  attr.Type = eMAT_Int;
  attr.Ints[0] = value;
}


void Material::Set(UInt16 idx, const Matrix3f& m)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute& attr = m_attributes[idx];
  attr.Type = eMAT_Matrix3;
  memcpy(attr.Floats, &m, sizeof(float) * 9);
}

void Material::Set(UInt16 idx, const Matrix4f& m)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute& attr = m_attributes[idx];
  attr.Type = eMAT_Matrix4;
  memcpy(attr.Floats, &m, sizeof(float) * 16);
}


}