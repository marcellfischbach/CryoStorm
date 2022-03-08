#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/types.hh>
#include <ceCore/class.hh>

#include <string>


namespace ce
{


struct WndDesc;
CE_CLASS()
struct CE_CORE_API iWindow : public iObject
{
  CE_CLASS_GEN;

  virtual ~iWindow() {  }

  virtual void SetTitle(const std::string& title) = 0;

  virtual void SetPosition (int16_t x, int16_t y) = 0;

  virtual void SetResolution(uint16_t width, uint16_t height) = 0;

  virtual void Show () = 0;

  virtual void Present () = 0;

  virtual void ProcessUpdates() = 0;

};

}

