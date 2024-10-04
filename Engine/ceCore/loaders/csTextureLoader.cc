

#include <ceCore/loaders/csTextureLoader.hh>
#include <ceCore/graphics/iDevice.hh>
#include <ceCore/graphics/csImage.hh>
#include <ceCore/graphics/iTexture.hh>
#include <ceCore/graphics/iTexture2D.hh>
#include <ceCore/graphics/iTexture2DArray.hh>
#include <ceCore/graphics/iTextureCube.hh>
#include <ceCore/graphics/csSamplers.hh>
#include <ceCore/csObjectRegistry.hh>
#include <ceCore/resource/csAssetManager.hh>


namespace cryo
{

csTextureLoader::csTextureLoader()
{
  AddValidFile<iTexture2D>("TEX2D");
  AddValidFile<iTexture2DArray>("TEX2DARRAY");
  AddValidFile<iTextureCube>("TEXCUBE");
}


iObject *csTextureLoader::Load(const csCryoFile * file, const Class *cls, const csResourceLocator &locator) const
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

iTexture2D *csTextureLoader::LoadTexture2D(const csCryoFileElement *textureElement, const csResourceLocator & locator)
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

  iTexture2D *texture = csObjectRegistry::Get<iDevice>()->CreateTexture(desc);
  texture->Data(image);
  if (sampler)
  {
    texture->SetSampler(sampler);
  }


  return texture;
}

iTexture2DArray *csTextureLoader::LoadTexture2DArray(const csCryoFileElement *textureElement, const csResourceLocator & locator)
{
  return nullptr;
}

iTextureCube *csTextureLoader::LoadTextureCube(const csCryoFileElement *textureElement, const csResourceLocator & locator)
{
  return nullptr;
}


csImage* csTextureLoader::LoadImage(const csCryoFileElement *imageElement, const csResourceLocator & locator)
{
  if (!imageElement || imageElement->GetNumberOfAttributes() != 1)
  {
    return nullptr;
  }

  auto imageName = imageElement->GetAttribute(0, "");

  return csAssetManager::Get()->Get<csImage>(csResourceLocator(locator, imageName));
}

void csTextureLoader::ColorCorrection(const csCryoFileElement * textureElement, csImage *image)
{
  const csCryoFileElement *element = textureElement->GetChild("colorCorrection");
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


iSampler *csTextureLoader::LoadSampler(const csCryoFileElement *samplerElement, const csResourceLocator & locator)
{
  if (!samplerElement || samplerElement->GetNumberOfAttributes() != 1)
  {
    return nullptr;
  }

  iSampler *sampler;
  auto samplerName = samplerElement->GetAttribute(0, "");
  if(samplerName == std::string("Default"))
  {
    sampler = csObjectRegistry::Get<csSamplers>()->GetDefault();
  }
  else if (samplerName == std::string("Test"))
  {
    sampler = csObjectRegistry::Get<csSamplers>()->GetTest();
  }
  else if (samplerName == std::string("NormalMap"))
  {
    sampler = csObjectRegistry::Get<csSamplers>()->GetNormalMap();
  }
  else if (samplerName == std::string ("UISprite"))
  {
    sampler = csObjectRegistry::Get<csSamplers>()->GetUISprite();
  }
  else
  {
    sampler = csAssetManager::Get()->Get<iSampler>(csResourceLocator(locator, samplerName));
  }


  return sampler;
}


ePixelFormat csTextureLoader::GetPixelFormatFrom(const csImage *image)
{
  return image->GetPixelFormat();
}



}