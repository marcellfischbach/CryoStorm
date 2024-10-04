
#pragma once


#include "ceCore/csCoreExport.hh"
#include "ceCore/csDefs.hh"
#include "iKeyboard.hh"
#include "iMouse.hh"
#include "ceCore/csClass.hh"


namespace cryo
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