
#pragma once


#include <spcCore/coreexport.hh>
#include <spcCore/defs.hh>
#include <spcCore/input/ikeyboard.hh>
#include <spcCore/input/imouse.hh>
#include <spcCore/class.hh>


namespace spc
{


SPC_CLASS()
struct SPC_CORE_API iInputSystem : public SPC_SUPER(iObject)
{
  SPC_CLASS_GEN;
  ~iInputSystem() override = default;

  SPC_NODISCARD virtual iKeyboard* GetKeyboard() const = 0;
  SPC_NODISCARD virtual iMouse* GetMouse() const = 0;

};

}