

#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/graphics/ePixelFormat.hh>
#include <csCore/resource/iAssetLoader.hh>
#include <csCore/resource/csBaseCSFAssetLoader.hh>

namespace cryo
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

  iObject* Load(const csCryoFile * file, const csClass * cls, const csResourceLocator & locator) const override;

private:

  static iTexture2D *LoadTexture2D(const csCryoFileElement *textureElement, const csResourceLocator & locator);
  static iTexture2DArray *LoadTexture2DArray(const csCryoFileElement *textureElement, const csResourceLocator & locator);
  static iTextureCube *LoadTextureCube(const csCryoFileElement *textureElement, const csResourceLocator & locator);
  static void ColorCorrection(const csCryoFileElement *textureElement, csImage* image);

  static csImage *LoadImage(const csCryoFileElement *imageElement, const csResourceLocator & locator);
  static iSampler *LoadSampler(const csCryoFileElement *samplerElement, const csResourceLocator & locator);

  static ePixelFormat GetPixelFormatFrom(const csImage *image);
};


}