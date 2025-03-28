

#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/graphics/ePixelFormat.hh>
#include <csCore/resource/csBaseCSFAssetLoader.hh>
#include <csCore/csRef.hh>

namespace cs
{

class csImage;
struct iTexture;
struct iTexture2D;
struct iTexture2DArray;
struct iTextureCube;
struct iSampler;


CS_CLASS()
class CS_CORE_API csTextureLoader : public CS_SUPER(csBaseCSFAssetLoader)
{
CS_CLASS_GEN_OBJECT;
public:
  csTextureLoader();
  virtual ~csTextureLoader() override = default;

  csOwned<iAsset> Load(const file::csCryoFile *file, const csAssetLocator &locator) const override;

private:

  static csOwned<iTexture2D> LoadTexture2D(const file::csCryoFileElement *textureElement, const csAssetLocator & locator);
  static csOwned<iTexture2DArray> LoadTexture2DArray(const file::csCryoFileElement *textureElement, const csAssetLocator & locator);
  static csOwned<iTextureCube> LoadTextureCube(const file::csCryoFileElement *textureElement, const csAssetLocator & locator);
  static void ColorCorrection(const file::csCryoFileElement *textureElement, csImage* image);

  static csRef<csImage> LoadImage(const file::csCryoFileElement *imageElement, const csAssetLocator & locator);
  static csRef<iSampler> LoadSampler(const file::csCryoFileElement *samplerElement, const csAssetLocator & locator);

  static ePixelFormat GetPixelFormatFrom(const csImage *image);
};


}