
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

class QWidget;
struct iEditorFactory
{
  virtual ~iEditorFactory() = default;

  virtual bool CanEdit(const cs::csAssetLocator &locator) const = 0;

  virtual void Edit(const cs::csAssetLocator &locator, QWidget *path = nullptr) const = 0;

};

