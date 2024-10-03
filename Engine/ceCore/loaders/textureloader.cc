

#include <ceCore/loaders/textureloader.hh>
#include <ceCore/graphics/iDevice.hh>
#include <ceCore/graphics/csImage.hh>
#include <ceCore/graphics/iTexture.hh>
#include <ceCore/graphics/iTexture2D.hh>
#include <ceCore/graphics/iTexture2DArray.hh>
#include <ceCore/graphics/iTextureCube.hh>
#include <ceCore/graphics/csSamplers.hh>
#include <ceCore/objectregistry.hh>
#include <ceCore/resource/assetmanager.hh>


namespace cryo
{

TextureLoader::TextureLoader()
{
  AddValidFile<iTexture2D>("TEX2D");
  AddValidFile<iTexture2DArray>("TEX2DARRAY");
  AddValidFile<iTextureCube>("TEXCUBE");
}


iObject *TextureLoader::Load(const CrimsonFile * file, const Class *cls, const ResourceLocator &locator) const
{
  auto textureElement = file->Root()->GetChild(0);
  auto tag = textureElement->GetTagName();
  if (tag == "texture2d")
  {
    return LoadTexture2D(textureElement, locator);
  }
  else if (tag == "texture2darray")
  {
    return LoadTexture2DArray(textureElement, locator);
  }
  else if (tag == "texturecube")
  {
    return LoadTextureCube(textureElement, locator);
  }

  return nullptr;
}

iTexture2D *TextureLoader::LoadTexture2D(const CrimsonFileElement *textureElement, const ResourceLocator & locator)
{

  auto image = LoadImage (textureElement->GetChild("image"), locator);
  if (!image)
  {
    return nullptr;
  }

  ColorCorrection(textureElement, image);

  iSampler *sampler = LoadSampler(textureElement->GetChild("sampler"), locator);



  iTexture2D::Descriptor desc = {};
  desc.Format = GetPixelFormatFrom(image);
  desc.Width = image->GetWidth();
  desc.Height = image->GetHeight();
  desc.MipMaps = textureElement->HasChild("mipmaps");

  if (desc.MipMaps)
  {
    // decide, based on the sampler type the mip mapping procedure
    image->GenerateMipMaps(csImage::eMipMapProcedure::eMMP_Linear4x4);
  }

  iTexture2D *texture = ObjectRegistry::Get<iDevice>()->CreateTexture(desc);
  texture->Data(image);
  if (sampler)
  {
    texture->SetSampler(sampler);
  }


  return texture;
}

iTexture2DArray *TextureLoader::LoadTexture2DArray(const CrimsonFileElement *textureElement, const ResourceLocator & locator)
{
  return nullptr;
}

iTextureCube *TextureLoader::LoadTextureCube(const CrimsonFileElement *textureElement, const ResourceLocator & locator)
{
  return nullptr;
}


csImage* TextureLoader::LoadImage(const CrimsonFileElement *imageElement, const ResourceLocator & locator)
{
  if (!imageElement || imageElement->GetNumberOfAttributes() != 1)
  {
    return nullptr;
  }

  auto imageName = imageElement->GetAttribute(0, "");

  return AssetManager::Get()->Get<csImage>(ResourceLocator(locator, imageName));
}

void TextureLoader::ColorCorrection(const CrimsonFileElement * textureElement, csImage *image)
{
  const CrimsonFileElement *element = textureElement->GetChild("colorCorrection");
  if (!element)
  {
    return;
  }

  csImage::eColorCorrection correction        = csImage::eColorCorrection::eCC_Plain;
  const std::string         &correctionString = element->GetAttribute(0, "Plain");
  if (correctionString == "Normalize")
  {
    correction = csImage::eColorCorrection::eCC_Normalize;
  }
  else if (correctionString == "Clamp3")
  {
    correction = csImage::eColorCorrection::eCC_Clamp3;
  }
  else if (correctionString == "Clamp4")
  {
    correction = csImage::eColorCorrection::eCC_Clamp4;
  }

  image->ColorCorrection(correction);
}


iSampler *TextureLoader::LoadSampler(const CrimsonFileElement *samplerElement, const ResourceLocator & locator)
{
  if (!samplerElement || samplerElement->GetNumberOfAttributes() != 1)
  {
    return nullptr;
  }

  iSampler *sampler;
  auto samplerName = samplerElement->GetAttribute(0, "");
  if(samplerName == std::string("Default"))
  {
    sampler = ObjectRegistry::Get<csSamplers>()->GetDefault();
  }
  else if (samplerName == std::string("Test"))
  {
    sampler = ObjectRegistry::Get<csSamplers>()->GetTest();
  }
  else if (samplerName == std::string("NormalMap"))
  {
    sampler = ObjectRegistry::Get<csSamplers>()->GetNormalMap();
  }
  else if (samplerName == std::string ("UISprite"))
  {
    sampler = ObjectRegistry::Get<csSamplers>()->GetUISprite();
  }
  else
  {
    sampler = AssetManager::Get()->Get<iSampler>(ResourceLocator(locator, samplerName));
  }


  return sampler;
}


ePixelFormat TextureLoader::GetPixelFormatFrom(const csImage *image)
{
  return image->GetPixelFormat();
}



}