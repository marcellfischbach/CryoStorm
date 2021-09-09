

#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/graphics/epixelformat.hh>
#include <spcCore/resource/iassetloader.hh>

namespace spc
{

class Image;
struct iTexture;
struct iTexture2D;
struct iTexture2DArray;
struct iTextureCube;
struct iSampler;

SPC_CLASS()
class SPC_CORE_API TextureLoaderSpc : public SPC_SUPER(iAssetLoaderSpc)
{
SPC_CLASS_GEN_OBJECT;
public:
  ~TextureLoaderSpc() override = default;


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