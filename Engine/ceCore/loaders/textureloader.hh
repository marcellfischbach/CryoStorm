

#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/graphics/epixelformat.hh>
#include <ceCore/resource/iassetloader.hh>

namespace ce
{

class Image;
struct iTexture;
struct iTexture2D;
struct iTexture2DArray;
struct iTextureCube;
struct iSampler;

CE_CLASS()
class CE_CORE_API TextureLoaderCEF : public CE_SUPER(iAssetLoaderCEF)
{
CE_CLASS_GEN_OBJECT;
public:
  ~TextureLoaderCEF() override = default;


  bool CanLoad(const Class * cls, const file::File * file, const ResourceLocator * locator) const override;

  iObject* Load(const Class * cls, const file::File * file, const ResourceLocator * locator) const override;

private:

  static iTexture2D *LoadTexture2D(const file::Element *textureElement);
  static iTexture2DArray *LoadTexture2DArray(const file::Element *textureElement);
  static iTextureCube *LoadTextureCube(const file::Element *textureElement);

  static Image *LoadImage (const file::Element *imageElement);
  static iSampler *LoadSampler(const file::Element *samplerElement);

  static ePixelFormat GetPixelFormatFrom(const Image *image);
};


}