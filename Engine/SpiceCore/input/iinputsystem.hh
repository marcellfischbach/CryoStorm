
#pragma once


#include <SpiceCore/coreexport.hh>
#include <SpiceCore/input/ikeyboard.hh>
#include <SpiceCore/input/imouse.hh>
#include <SpiceCore/class.hh>


namespace Spice
{


SPICE_CLASS()
struct SPICE_CORE_API iInputSystem : public SPICE_SUPER(iObject)
{
  SPICE_CLASS_GEN;

  virtual ~iInputSystem() { }

  virtual const iKeyboard* GetKeyboard() const = 0;
  virtual const iMouse* GetMouse() const = 0;

};

}