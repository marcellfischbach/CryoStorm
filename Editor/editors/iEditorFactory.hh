
#pragma once

#include <filesystem>
#include <csCore/resource/csAssetLocator.hh>
namespace std
{
namespace fs = filesystem;
}

namespace cs
{
struct iObject;
}

class EditorWidget;
class QWidget;
struct iEditorFactory
{
  virtual ~iEditorFactory() = default;

  virtual bool CanEdit(const cs::csAssetLocator &locator) const = 0;

  virtual EditorWidget *Create(const cs::csAssetLocator &locator) const = 0;

};

