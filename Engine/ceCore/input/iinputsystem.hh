
#pragma once


#include <ceCore/coreexport.hh>
#include <ceCore/defs.hh>
#include <ceCore/input/ikeyboard.hh>
#include <ceCore/input/imouse.hh>
#include <ceCore/class.hh>


namespace ce
{


CE_CLASS()
struct CE_CORE_API iInputSystem : public CE_SUPER(iObject)
{
  CE_CLASS_GEN;
  ~iInputSystem() override = default;

  CE_NODISCARD virtual iKeyboard* GetKeyboard() const = 0;
  CE_NODISCARD virtual iMouse* GetMouse() const = 0;

};

}