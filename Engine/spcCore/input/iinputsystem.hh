
#pragma once


#include <spcCore/coreexport.hh>
#include <spcCore/input/ikeyboard.hh>
#include <spcCore/input/imouse.hh>
#include <spcCore/class.hh>


namespace spc
{


SPC_CLASS()
struct SPC_CORE_API iInputSystem : public SPC_SUPER(iObject)
{
  SPC_CLASS_GEN;

  virtual ~iInputSystem() { }

  virtual const iKeyboard* GetKeyboard() const = 0;
  virtual const iMouse* GetMouse() const = 0;

};

}