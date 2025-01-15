

#include <csCore/loaders/csTextureLoader.hh>
#include <csCore/graphics/iDevice.hh>
#include <csCore/graphics/csImage.hh>
#include <csCore/graphics/iTexture.hh>
#include <csCore/graphics/iTexture2D.hh>
#include <csCore/graphics/iTexture2DArray.hh>
#include <csCore/graphics/iTextureCube.hh>
#include <csCore/graphics/csSamplers.hh>
#include <csCore/csObjectRegistry.hh>
#include <csCore/resource/csAssetManager.hh>


namespace cs
{

csTextureLoader::csTextureLoader()
{
  RegisterType("TEX2D");
  RegisterType("TEX2DARRAY");
  RegisterType("TEXCUBE");

}


csOwned<iAsset>csTextureLoader::Load(const csCryoFile *file, const csAssetLocator &locator) const
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

csOwned<iTexture2D> csTextureLoader::LoadTexture2D(const csCryoFileElement *textureElement, const csAssetLocator & locator)
{

  auto image = LoadImage (textureElement->GetChild("image"), locator);
  if (!image)
  {
    return nullptr;
  }

  ColorCorrection(textureElement, image);

  auto sampler = LoadSampler(textureElement->GetChild("sampler"), locator);



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

  auto texture = csObjectRegistry::Get<iDevice>()->CreateTexture(desc);
  texture->Data(image);
  if (sampler)
  {
    texture->SetSampler(sampler);
  }


  return texture;
}

csOwned<iTexture2DArray> csTextureLoader::LoadTexture2DArray(const csCryoFileElement *textureElement, const csAssetLocator & locator)
{
  return nullptr;
}

csOwned<iTextureCube> csTextureLoader::LoadTextureCube(const csCryoFileElement *textureElement, const csAssetLocator & locator)
{
  return nullptr;
}


csRef<csImage> csTextureLoader::LoadImage(const csCryoFileElement *imageElement, const csAssetLocator & locator)
{
  if (!imageElement || imageElement->GetNumberOfAttributes() != 1)
  {
    return nullptr;
  }

  auto imageName = imageElement->GetAttribute(0, "");

  return csRef<csImage>(csAssetManager::Get()->Get<csImage>(csAssetLocator(locator, imageName)));
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


csRef<iSampler> csTextureLoader::LoadSampler(const csCryoFileElement *samplerElement, const csAssetLocator & locator)
{
  if (!samplerElement || samplerElement->GetNumberOfAttributes() != 1)
  {
    return nullptr;
  }

  csRef<iSampler> sampler;
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
    sampler = csAssetManager::Get()->Get<iSampler>(csAssetLocator(locator, samplerName)).Data();
  }


  return sampler;
}


ePixelFormat csTextureLoader::GetPixelFormatFrom(const csImage *image)
{
  return image->GetPixelFormat();
}



}