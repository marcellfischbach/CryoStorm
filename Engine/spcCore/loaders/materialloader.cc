
#include <spcCore/loaders/materialloader.hh>
#include <spcCore/graphics/material/material.hh>
#include <spcCore/graphics/material/materialinstance.hh>
#include <spcCore/graphics/shading/ishader.hh>
#include <spcCore/resource/assetmanager.hh>

namespace spc
{


bool MaterialLoaderSpc::CanLoad(const Class* cls, const file::File* file, const ResourceLocator* locator) const
{
  return cls == Material::GetStaticClass() && file && file->Root()->HasChild("material")
    || cls == MaterialInstance::GetStaticClass() && file && file->Root()->HasChild("materialinstance");
}

iObject* MaterialLoaderSpc::Load(const Class* cls, const file::File* file, const ResourceLocator* locator) const
{
  if (cls == Material::GetStaticClass() && file && file->Root()->HasChild("material"))
  {
    return LoadMaterial(cls, file, locator);
  }
  else if (cls == MaterialInstance::GetStaticClass() && file && file->Root()->HasChild("materialinstance"))
  {
    return LoadMaterialInstance(cls, file, locator);
  }
  return nullptr;
}


Material* MaterialLoaderSpc::LoadMaterial(const Class* cls, const file::File* file, const ResourceLocator* locator) const
{
  const file::Element* root = file->Root();
  const file::Element* materialElement = root->GetChild("material");
  if (!materialElement)
  {
    return nullptr;
  }

  Material* material = new Material();

  if (!LoadShaders(material, materialElement))
  {
    SPC_RELEASE(material);
    return nullptr;
  }

  if (!LoadAttributes(material, materialElement))
  {
    SPC_RELEASE(material);
    return nullptr;
  }


  return material;
}


bool MaterialLoaderSpc::LoadShaders(Material* material, const file::Element* materialElement) const
{
  const file::Element* shadersElement = materialElement->GetChild("shaders");
  if (!shadersElement)
  {
    return false;
  }

  for (size_t i = 0; i < shadersElement->GetNumberOfChildren(); ++i)
  {
    const file::Element* shaderElement = shadersElement->GetChild(i);
    if (shaderElement && shaderElement->GetTagName() == std::string("shader"))
    {
      if (!LoadShader(material, shaderElement))
      {
        return false;
      }
    }
  }
  return true;
}



#define IF(prefix, name, text) if (std::string(#name) == text) return prefix##name

eRenderPass RenderPass(const std::string& renderPass)
{
#define IF_(name) IF(eRP_, name, renderPass)
#define ELSE_IF_(name) else IF(eRP_, name, renderPass)
  IF_(Depth);
  ELSE_IF_(GBuffer);
  ELSE_IF_(Forward);
  ELSE_IF_(Shadow);
  ELSE_IF_(ShadowPSSM);
  ELSE_IF_(ShadowCube);
#undef IF_
#undef ELSE_IF_
  return eRP_COUNT;
}


bool MaterialLoaderSpc::LoadShader(Material* material, const file::Element* shaderElement) const
{
  if (shaderElement->GetNumberOfAttributes() != 2)
  {
    return false;
  }

  std::string rp = shaderElement->GetAttribute(0, std::string(""));
  std::string shaderLoc = shaderElement->GetAttribute(1, std::string(""));

  if (rp.empty() || shaderLoc.empty())
  {
    return false;
  }

  eRenderPass renderPass = RenderPass(rp);
  if (renderPass == eRP_COUNT)
  {
    return false;
  }

  ResourceLocator shaderLocator(shaderLoc);
  iShader* shader = AssetManager::Get()->Get<iShader>(shaderLocator);
  if (!shader)
  {
    return false;
  }


  material->SetShader(renderPass, shader);

  return true;
}

bool MaterialLoaderSpc::LoadAttributes(Material* material, const file::Element* materialElement) const
{
  const file::Element* attributesElement = materialElement->GetChild("attributes");
  if (!attributesElement)
  {
    return true;
  }

  for (size_t i = 0; i < attributesElement->GetNumberOfChildren(); ++i)
  {
    const file::Element* attributeElement = attributesElement->GetChild(i);
    if (attributeElement && attributeElement->GetTagName() == std::string("attribute"))
    {
      if (!LoadAttribute(material, attributeElement))
      {
        return false;
      }
    }
  }
  return true;
}



iObject* MaterialLoaderSpc::LoadMaterialInstance(const Class* cls, const file::File* file, const ResourceLocator* locator) const
{
  return nullptr;
}





bool MaterialLoaderSpc::LoadAttribute(Material* material, const file::Element* attributeElement) const
{
  if (attributeElement->GetNumberOfAttributes() == 0)
  {
    return false;
  }

  std::string name = attributeElement->GetAttribute(0, "");
  if (name.empty())
  {
    return false;
  }

  material->RegisterAttribute(name);
  size_t idx = material->IndexOf(name);

  if (!LoadAttributeDefault(material, idx, attributeElement))
  {
    return false;
  }


  return true;
}

bool MaterialLoaderSpc::LoadAttributeDefault(iMaterial* material, size_t attributeIdx, const file::Element* attributeElement) const
{
  if (attributeElement->GetNumberOfAttributes() < 2)
  {
    // no default defined... thats ok
    return true;
  }
  std::string type = attributeElement->GetAttribute(1, "");
  if (type == "Float")
  {
    LoadAttributeFloat(material, attributeIdx, attributeElement);
  }
  else if (type == "Vec2")
  {
    LoadAttributeVec2(material, attributeIdx, attributeElement);
  }
  else if (type == "Vec3")
  {
    LoadAttributeVec3(material, attributeIdx, attributeElement);
  }
  else if (type == "Vec4")
  {
    LoadAttributeVec4(material, attributeIdx, attributeElement);
  }
  else if (type == "Color4")
  {
    LoadAttributeColor4(material, attributeIdx, attributeElement);
  }
  else if (type == "Int")
  {
    LoadAttributeInt(material, attributeIdx, attributeElement);
  }
  else if (type == "Matrix3")
  {
    LoadAttributeMatrix3(material, attributeIdx, attributeElement);
  }
  else if (type == "Matrix4")
  {
    LoadAttributeMatrix4(material, attributeIdx, attributeElement);
  }
  else if (type == "Texture")
  {
    LoadAttributeTexture(material, attributeIdx, attributeElement);
  }
}



bool MaterialLoaderSpc::LoadAttributeFloat(iMaterial* material, size_t attributeIdx, const file::Element* attributeElement) const 
{ 
  if (attributeElement->GetNumberOfAttributes() < 3)
  {
    return false;
  }

  double v0 = attributeElement->GetAttribute(2, 0.0);
  material->Set(attributeIdx, (float)v0);
  return true;
}

bool MaterialLoaderSpc::LoadAttributeVec2(iMaterial* material, size_t attributeIdx, const file::Element* attributeElement) const 
{
  if (attributeElement->GetNumberOfAttributes() < 4)
  {
    return false;
  }

  double v0 = attributeElement->GetAttribute(2, 0.0);
  double v1 = attributeElement->GetAttribute(3, 0.0);
  material->Set(attributeIdx, Vector2f(v0, v1));
  return true;
}

bool MaterialLoaderSpc::LoadAttributeVec3(iMaterial* material, size_t attributeIdx, const file::Element* attributeElement) const 
{
  if (attributeElement->GetNumberOfAttributes() < 5)
  {
    return false;
  }

  double v0 = attributeElement->GetAttribute(2, 0.0);
  double v1 = attributeElement->GetAttribute(3, 0.0);
  double v2 = attributeElement->GetAttribute(4, 0.0);
  material->Set(attributeIdx, Vector3f(v0, v1, v2));
  return true;
}


bool MaterialLoaderSpc::LoadAttributeVec4(iMaterial* material, size_t attributeIdx, const file::Element* attributeElement) const 
{
  if (attributeElement->GetNumberOfAttributes() < 6)
  {
    return false;
  }

  double v0 = attributeElement->GetAttribute(2, 0.0);
  double v1 = attributeElement->GetAttribute(3, 0.0);
  double v2 = attributeElement->GetAttribute(4, 0.0);
  double v3 = attributeElement->GetAttribute(5, 0.0);
  material->Set(attributeIdx, Vector4f(v0, v1, v2, v3));
  return true;
}

bool MaterialLoaderSpc::LoadAttributeColor4(iMaterial* material, size_t attributeIdx, const file::Element* attributeElement) const 
{
  if (attributeElement->GetNumberOfAttributes() < 6)
  {
    return false;
  }

  double v0 = attributeElement->GetAttribute(2, 0.0);
  double v1 = attributeElement->GetAttribute(3, 0.0);
  double v2 = attributeElement->GetAttribute(4, 0.0);
  double v3 = attributeElement->GetAttribute(5, 0.0);
  material->Set(attributeIdx, Color4f(v0, v1, v2, v3));
  return true;
}

bool MaterialLoaderSpc::LoadAttributeInt(iMaterial* material, size_t attributeIdx, const file::Element* attributeElement) const 
{
  if (attributeElement->GetNumberOfAttributes() < 3)
  {
    return false;
  }

  int v0 = attributeElement->GetAttribute(2, 0);
  material->Set(attributeIdx, v0);
  return true;
}

bool MaterialLoaderSpc::LoadAttributeMatrix3(iMaterial* material, size_t attributeIdx, const file::Element* attributeElement) const 
{
  if ( !attributeElement->HasAttribute("m00")
    || !attributeElement->HasAttribute("m01")
    || !attributeElement->HasAttribute("m02")
    || !attributeElement->HasAttribute("m10")
    || !attributeElement->HasAttribute("m11")
    || !attributeElement->HasAttribute("m12")
    || !attributeElement->HasAttribute("m20")
    || !attributeElement->HasAttribute("m21")
    || !attributeElement->HasAttribute("m22")
    ) 
  {
    return false;
  }

  float m00 = attributeElement->GetAttribute("m00", 1.0);
  float m01 = attributeElement->GetAttribute("m01", 0.0);
  float m02 = attributeElement->GetAttribute("m02", 0.0);
  float m10 = attributeElement->GetAttribute("m10", 0.0);
  float m11 = attributeElement->GetAttribute("m11", 1.0);
  float m12 = attributeElement->GetAttribute("m12", 0.0);
  float m20 = attributeElement->GetAttribute("m20", 0.0);
  float m21 = attributeElement->GetAttribute("m21", 0.0);
  float m22 = attributeElement->GetAttribute("m22", 1.0);
  material->Set(attributeIdx, Matrix3f(
    m00, m01, m02,
    m10, m11, m12,
    m20, m21, m22
  ));
}

bool MaterialLoaderSpc::LoadAttributeMatrix4(iMaterial* material, size_t attributeIdx, const file::Element* attributeElement) const 
{ 
  if (!attributeElement->HasAttribute("m00")
    || !attributeElement->HasAttribute("m01")
    || !attributeElement->HasAttribute("m02")
    || !attributeElement->HasAttribute("m03")
    || !attributeElement->HasAttribute("m10")
    || !attributeElement->HasAttribute("m11")
    || !attributeElement->HasAttribute("m12")
    || !attributeElement->HasAttribute("m13")
    || !attributeElement->HasAttribute("m20")
    || !attributeElement->HasAttribute("m21")
    || !attributeElement->HasAttribute("m22")
    || !attributeElement->HasAttribute("m23")
    || !attributeElement->HasAttribute("m30")
    || !attributeElement->HasAttribute("m31")
    || !attributeElement->HasAttribute("m32")
    || !attributeElement->HasAttribute("m33")
    )
  {
    return false;
  }

  float m00 = attributeElement->GetAttribute("m00", 1.0);
  float m01 = attributeElement->GetAttribute("m01", 0.0);
  float m02 = attributeElement->GetAttribute("m02", 0.0);
  float m03 = attributeElement->GetAttribute("m03", 0.0);
  float m10 = attributeElement->GetAttribute("m10", 0.0);
  float m11 = attributeElement->GetAttribute("m11", 1.0);
  float m12 = attributeElement->GetAttribute("m12", 0.0);
  float m13 = attributeElement->GetAttribute("m13", 0.0);
  float m20 = attributeElement->GetAttribute("m20", 0.0);
  float m21 = attributeElement->GetAttribute("m21", 0.0);
  float m22 = attributeElement->GetAttribute("m22", 1.0);
  float m23 = attributeElement->GetAttribute("m23", 0.0);
  float m30 = attributeElement->GetAttribute("m30", 0.0);
  float m31 = attributeElement->GetAttribute("m31", 0.0);
  float m32 = attributeElement->GetAttribute("m32", 0.0);
  float m33 = attributeElement->GetAttribute("m33", 1.0);
  material->Set(attributeIdx, Matrix4f(
    m00, m01, m02, m03,
    m10, m11, m12, m13,
    m20, m21, m22, m23,
    m30, m31, m32, m33
  ));
}

bool MaterialLoaderSpc::LoadAttributeTexture(iMaterial* material, size_t attributeIdx, const file::Element* attributeElement) const 
{
  if (attributeElement->GetNumberOfAttributes() < 3)
  {
    return false;
  }

  std::string textureLoc = attributeElement->GetAttribute(3, "");
  if (textureLoc.empty())
  {
    return false;
  }

  iTexture* texture = AssetManager::Get()->Get<iTexture>(ResourceLocator(textureLoc));
  if (!texture)
  {
    return false;
  }

  material->Set(attributeIdx, texture);
}


}