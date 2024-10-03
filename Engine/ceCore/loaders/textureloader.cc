

#include <ceCore/loaders/textureloader.hh>
#include <ceCore/graphics/idevice.hh>
#include <ceCore/graphics/image.hh>
#include <ceCore/graphics/itexture.hh>
#include <ceCore/graphics/itexture2d.hh>
#include <ceCore/graphics/itexture2darray.hh>
#include <ceCore/graphics/itexturecube.hh>
#include <ceCore/graphics/samplers.hh>
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
    image->GenerateMipMaps(Image::eMipMapProcedure::eMMP_Linear4x4);
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


Image* TextureLoader::LoadImage(const CrimsonFileElement *imageElement, const ResourceLocator & locator)
{
  if (!imageElement || imageElement->GetNumberOfAttributes() != 1)
  {
    return nullptr;
  }

  auto imageName = imageElement->GetAttribute(0, "");

  return AssetManager::Get()->Get<Image>(ResourceLocator(locator, imageName));
}

void TextureLoader::ColorCorrection(const CrimsonFileElement * textureElement, Image *image)
{
  const CrimsonFileElement *element = textureElement->GetChild("colorCorrection");
  if (!element)
  {
    return;
  }

  Image::eColorCorrection correction = Image::eColorCorrection::eCC_Plain;
  const std::string &correctionString = element->GetAttribute(0, "Plain");
  if (correctionString == "Normalize")
  {
    correction = Image::eColorCorrection::eCC_Normalize;
  }
  else if (correctionString == "Clamp3")
  {
    correction = Image::eColorCorrection::eCC_Clamp3;
  }
  else if (correctionString == "Clamp4")
  {
    correction = Image::eColorCorrection::eCC_Clamp4;
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
    sampler = ObjectRegistry::Get<Samplers>()->GetDefault();
  }
  else if (samplerName == std::string("Test"))
  {
    sampler = ObjectRegistry::Get<Samplers>()->GetTest();
  }
  else if (samplerName == std::string("NormalMap"))
  {
    sampler = ObjectRegistry::Get<Samplers>()->GetNormalMap();
  }
  else if (samplerName == std::string ("UISprite"))
  {
    sampler = ObjectRegistry::Get<Samplers>()->GetUISprite();
  }
  else
  {
    sampler = AssetManager::Get()->Get<iSampler>(ResourceLocator(locator, samplerName));
  }


  return sampler;
}


ePixelFormat TextureLoader::GetPixelFormatFrom(const Image *image)
{
  return image->GetPixelFormat();
}



}