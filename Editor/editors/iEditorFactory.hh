
#pragma once

#include <filesystem>
#include <csCore/resource/csResourceLocator.hh>
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

  virtual bool CanEdit(const cs::csResourceLocator &locator) const = 0;

  virtual void Edit(const cs::csResourceLocator &locator, QWidget *path = nullptr) const = 0;

};

