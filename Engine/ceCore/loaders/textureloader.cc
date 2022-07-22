

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


namespace ce
{


bool TextureLoaderCEF::CanLoad(const Class *cls, const CrimsonFile *file, const ResourceLocator *locator) const
{
  if (file->Root()->GetNumberOfChildren() == 0)
  {
    return false;
  }
  std::string root = file->Root()->GetChild(0)->GetTagName();
  return
      cls->IsAssignableFrom<iTexture2D>() && root == std::string("texture2d")
      || cls->IsAssignableFrom<iTexture2DArray>() && root == std::string("texture2darray")
      || cls->IsAssignableFrom<iTextureCube>() && root == std::string("texturecube")
      || cls->IsAssignableFrom<iTexture>() && (
          root == std::string("texture2d")
          || root == std::string("texture2darray")
          || root == std::string("texturecube")
      );
}

iObject *TextureLoaderCEF::Load(const Class *cls, const CrimsonFile *file, const ResourceLocator *locator) const
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

iTexture2D *TextureLoaderCEF::LoadTexture2D(const CrimsonFileElement *textureElement, const ResourceLocator * locator)
{

  auto image = LoadImage (textureElement->GetChild("image"), locator);
  if (!image)
  {
    return nullptr;
  }

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

iTexture2DArray *TextureLoaderCEF::LoadTexture2DArray(const CrimsonFileElement *textureElement, const ResourceLocator * locator)
{
  return nullptr;
}

iTextureCube *TextureLoaderCEF::LoadTextureCube(const CrimsonFileElement *textureElement, const ResourceLocator * locator)
{
  return nullptr;
}


Image* TextureLoaderCEF::LoadImage(const CrimsonFileElement *imageElement, const ResourceLocator * locator)
{
  if (!imageElement || imageElement->GetNumberOfAttributes() != 1)
  {
    return nullptr;
  }

  auto imageName = imageElement->GetAttribute(0, "");

  return AssetManager::Get()->Get<Image>(ResourceLocator(locator, imageName));
}

iSampler *TextureLoaderCEF::LoadSampler(const CrimsonFileElement *samplerElement, const ResourceLocator * locator)
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


ePixelFormat TextureLoaderCEF::GetPixelFormatFrom(const Image *image)
{
  return image->GetPixelFormat();
}

















bool TextureLoader::CanLoad(const Class *cls, const ResourceLocator &locator) const
{
  std::string ext =locator.GetExtension();

  return
    cls->IsAssignableFrom<iTexture2D>() && ext == std::string("TEX2D")
      || cls->IsAssignableFrom<iTexture2DArray>() && ext == std::string("TEX2DARRAY")
      || cls->IsAssignableFrom<iTextureCube>() && ext == std::string("TEXCUBE")
      || cls->IsAssignableFrom<iTexture>() && (
        ext == std::string("TEX2D")
          || ext == std::string("TEX2DARRAY")
          || ext == std::string("TEXCUBE")
      );
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