
#include <spcCore/graphics/material/material.hh>
#include <spcCore/graphics/shading/ishader.hh>
#include <spcCore/graphics/shading/ishaderattribute.hh>


namespace spc
{

Material::Material()
  : iMaterial()
  , m_queue(eRenderQueue::Default)
{
  SPC_CLASS_GEN_CONSTR;

  for (Size i = 0; i < eRP_COUNT; i++)
  {
    m_shader[i] = nullptr;
  }
}


Material::~Material()
{
  for (Size i = 0; i < eRP_COUNT; i++)
  {
    SPC_RELEASE(m_shader[i]);
    m_shader[i] = nullptr;
  }

  for (auto attribute : m_attributes)
  {
    SPC_RELEASE(attribute.Texture);
  }
}

void Material::SetRenderQueue(eRenderQueue queue)
{
  m_queue = queue;
}

eRenderQueue Material::GetRenderQueue() const
{
  return m_queue;
}

bool Material::Bind(iDevice* device, eRenderPass pass)
{
  if (!BindShader(device, pass))
  {
    return false;
  }
  device->ResetTextures();
  for (Size i = 0, in = m_attributes.size(); i < in; ++i)
  {
    BindAttribute(device, pass, i);
  }
  return true;
}

bool Material::BindShader(iDevice* device, eRenderPass pass)
{
  iShader* shader = m_shader[pass];
  if (!shader)
  {
    return false;
  }

  device->SetShader(shader);
  return true;
}


void Material::BindAttribute(iDevice* device, eRenderPass pass, Size idx)
{
  Attribute& attr = m_attributes[idx];
  BindAttribute(device, pass, idx, attr.Floats, attr.Ints, attr.Texture);
}

void Material::BindAttribute(iDevice *device, eRenderPass pass, Size idx, float *floats, int* ints, iTexture*texture)
{
  Attribute& attribute = m_attributes[idx];
  iShaderAttribute* shaderAttribute = attribute.Attributes[pass];
  if (shaderAttribute)
  {
    switch (attribute.Type)
    {
    case eMAT_Float:
      shaderAttribute->Bind(floats[0]);
      break;
    case eMAT_Vec2:
      shaderAttribute->Bind(*reinterpret_cast<Vector2f*>(floats));
      break;
    case eMAT_Vec3:
      shaderAttribute->Bind(*reinterpret_cast<Vector3f*>(floats));
      break;
    case eMAT_Vec4:
      shaderAttribute->Bind(*reinterpret_cast<Vector4f*>(floats));
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
      shaderAttribute->Bind(*reinterpret_cast<Matrix3f*>(floats));
      break;
    case eMAT_Matrix4:
      shaderAttribute->Bind(*reinterpret_cast<Matrix4f*>(floats));
      break;
    case eMAT_Texture:
      eTextureUnit unit = device->BindTexture(texture);
      if (unit != eTU_Invalid)
      {
        shaderAttribute->Bind(unit);
      }
      break;
    }
  }
}



void Material::SetShader(eRenderPass pass, iShader* shader)
{
  SPC_SET(m_shader[pass], shader);

  UpdateShaderAttributes(pass);
}

iShader* Material::GetShader(eRenderPass pass)
{
  return m_shader[pass];
}


const iShader* Material::GetShader(eRenderPass pass) const
{
  return m_shader[pass];
}




void Material::RegisterAttribute(const std::string& attributeName)
{
  Attribute attribute;
  //memset(&attribute, 0, sizeof(Attribute));
  attribute.Name = attributeName;
  attribute.Texture = nullptr;
  for (int i = 0; i < eRP_COUNT; ++i)
  {
    attribute.Attributes[i] = m_shader[i] ? m_shader[i]->GetShaderAttribute(attributeName) : nullptr;
  }
  attribute.Type = eMAT_Undefined;
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
  for (const Attribute& attr : m_attributes)
  {
    names.push_back(attr.Name);
  }
  return names;
}

Size Material::IndexOf(const std::string& attributeName)
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
  Attribute& attr = m_attributes[idx];
  attr.Type = eMAT_Float;
  attr.Floats[0] = value;
}


void Material::Set(Size idx, const Vector2f& v)
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


void Material::Set(Size idx, const Vector3f& v)
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

void Material::Set(Size idx, const Vector4f& v)
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

void Material::Set(Size idx, const Color4f& v)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute& attr = m_attributes[idx];
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
  Attribute& attr = m_attributes[idx];
  attr.Type = eMAT_Int;
  attr.Ints[0] = value;
}


void Material::Set(Size idx, const Matrix3f& m)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute& attr = m_attributes[idx];
  attr.Type = eMAT_Matrix3;
  memcpy(attr.Floats, &m, sizeof(float) * 9);
}

void Material::Set(Size idx, const Matrix4f& m)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute& attr = m_attributes[idx];
  attr.Type = eMAT_Matrix4;
  memcpy(attr.Floats, &m, sizeof(float) * 16);
}

void Material::Set(Size idx, iTexture* texture)
{
  if (idx >= m_attributes.size())
  {
    return;
  }
  Attribute& attr = m_attributes[idx];
  attr.Type = eMAT_Texture;
  SPC_SET(attr.Texture, texture);

}


}