#pragma once

#include <cecore/cetypes.hh>

struct iWindow
{
  virtual void Set(const csWndDesc& desc) = 0;

  virtual void Create() = 0;

  virtual void SetVisible(bool visible) = 0;

  virtual void Update() = 0;

  virtual void Present() = 0;
};