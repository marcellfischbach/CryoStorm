#pragma once

#include <SpiceCore/coreexport.hh>
#include <SpiceCore/types.hh>
#include <SpiceCore/class.hh>
#include <SpiceCore/window/iwindow.refl.hh>

#include <string>


namespace Spice
{


struct WndDesc;
SPICE_CLASS()
struct SPICE_CORE_API iWindow : public iObject
{
  SPICE_CLASS_GEN;

  virtual ~iWindow() {  }

  virtual void SetTitle(const std::string& title) = 0;

  virtual void SetPosition (Int16 x, Int16 y) = 0;

  virtual void SetResolution(UInt16 width, UInt16 height) = 0;

  virtual void Show () = 0;

  virtual void Present () = 0;

  virtual void ProcessUpdates() = 0;

};

}

