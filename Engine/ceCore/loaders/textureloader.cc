

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


bool TextureLoaderCEF::CanLoad(const Class *cls, const file::File *file, const ResourceLocator *locator) const
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

iObject *TextureLoaderCEF::Load(const Class *cls, const file::File *file, const ResourceLocator *locator) const
{
  auto textureElement = file->Root()->GetChild(0);
  auto tag = textureElement->GetTagName();
  if (tag == "texture2d")
  {
    return LoadTexture2D(textureElement);
  }
  else if (tag == "texture2darray")
  {
    return LoadTexture2DArray(textureElement);
  }
  else if (tag == "texturecube")
  {
    return LoadTextureCube(textureElement);
  }

  return nullptr;
}

iTexture2D *TextureLoaderCEF::LoadTexture2D(const file::Element *textureElement)
{

  auto image = LoadImage (textureElement->GetChild("image"));
  if (!image)
  {
    return nullptr;
  }

  iSampler *sampler = LoadSampler(textureElement->GetChild("sampler"));

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

iTexture2DArray *TextureLoaderCEF::LoadTexture2DArray(const file::Element *textureElement)
{
  return nullptr;
}

iTextureCube *TextureLoaderCEF::LoadTextureCube(const file::Element *textureElement)
{
  return nullptr;
}


Image* TextureLoaderCEF::LoadImage(const file::Element *imageElement)
{
  if (!imageElement || imageElement->GetNumberOfAttributes() != 1)
  {
    return nullptr;
  }

  auto imageName = imageElement->GetAttribute(0, "");

  return AssetManager::Get()->Get<Image>(ResourceLocator(imageName));
}

iSampler *TextureLoaderCEF::LoadSampler(const file::Element *samplerElement)
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
    sampler = AssetManager::Get()->Get<iSampler>(ResourceLocator(samplerName));
  }


  return sampler;
}


ePixelFormat TextureLoaderCEF::GetPixelFormatFrom(const Image *image)
{
  return image->GetPixelFormat();
}



}