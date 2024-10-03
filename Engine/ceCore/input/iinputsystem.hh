
#pragma once


#include "ceCore/coreexport.hh"
#include "ceCore/defs.hh"
#include "ikeyboard.hh"
#include "imouse.hh"
#include "ceCore/class.hh"


namespace ce
{


CS_CLASS()
struct CS_CORE_API iInputSystem : public CS_SUPER(iObject)
{
  CS_CLASS_GEN;
  ~iInputSystem() override = default;

  CS_NODISCARD virtual iKeyboard* GetKeyboard() const = 0;
  CS_NODISCARD virtual iMouse* GetMouse() const = 0;


};

}