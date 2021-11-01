#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/types.hh>
#include <spcCore/class.hh>

#include <string>


namespace spc
{


struct WndDesc;
SPC_CLASS()
struct SPC_CORE_API iWindow : public iObject
{
  SPC_CLASS_GEN;

  virtual ~iWindow() {  }

  virtual void SetTitle(const std::string& title) = 0;

  virtual void SetPosition (Int16 x, Int16 y) = 0;

  virtual void SetResolution(uint16_t width, uint16_t height) = 0;

  virtual void Show () = 0;

  virtual void Present () = 0;

  virtual void ProcessUpdates() = 0;

};

}

