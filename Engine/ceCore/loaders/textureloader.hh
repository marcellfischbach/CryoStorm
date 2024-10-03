

#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/graphics/epixelformat.hh>
#include <ceCore/resource/iassetloader.hh>
#include <ceCore/resource/basecefassetloader.hh>

namespace cryo
{

class Image;
struct iTexture;
struct iTexture2D;
struct iTexture2DArray;
struct iTextureCube;
struct iSampler;


CS_CLASS()
class CS_CORE_API TextureLoader : public CS_SUPER(BaseCEFAssetLoader)
{
CS_CLASS_GEN_OBJECT;
public:
  TextureLoader();
  virtual ~TextureLoader() override = default;

  iObject* Load(const CrimsonFile * file, const Class * cls, const ResourceLocator & locator) const override;

private:

  static iTexture2D *LoadTexture2D(const CrimsonFileElement *textureElement, const ResourceLocator & locator);
  static iTexture2DArray *LoadTexture2DArray(const CrimsonFileElement *textureElement, const ResourceLocator & locator);
  static iTextureCube *LoadTextureCube(const CrimsonFileElement *textureElement, const ResourceLocator & locator);
  static void ColorCorrection(const CrimsonFileElement *textureElement, Image* image);

  static Image *LoadImage (const CrimsonFileElement *imageElement, const ResourceLocator & locator);
  static iSampler *LoadSampler(const CrimsonFileElement *samplerElement, const ResourceLocator & locator);

  static ePixelFormat GetPixelFormatFrom(const Image *image);
};


}