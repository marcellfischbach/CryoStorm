
#include <csEditor/editors/image/ImageEditorFactory.hh>
#include <csEditor/editors/image/ImageEditorWidget.hh>
#include <csCore/resource/csAssetManager.hh>
#include <csCore/graphics/csImage.hh>

using namespace cs;

bool ImageEditorFactory::CanEdit(const cs::csAssetLocator &locator) const
{
  std::string ext = locator.GetExtension();

  return ext == "JPG"
         || ext == "JPEG"
         || ext == "PNG";
}



EditorWidget *ImageEditorFactory::Create(const cs::csAssetLocator &locator) const
{
  auto image = csAssetManager::Get()->Load<csImage>(locator);
  if (!image)
  {
    return nullptr;
  }

  return new ImageEditorWidget(image.Data());
}
