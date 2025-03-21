
#pragma once


#include <csCore/csCoreExport.hh>
#include <csCore/csDefs.hh>
#include <csCore/input/iKeyboard.hh>
#include <csCore/input/iMouse.hh>
#include <csCore/csClass.hh>


namespace cs
{


CS_CLASS(jclass="org.cryo.core.input.IInputSystem")
struct CS_CORE_API iInputSystem : public CS_SUPER(iObject)
{
  CS_CLASS_GEN;
  ~iInputSystem() override = default;

  CS_FUNCTION()
  CS_NODISCARD virtual cs::iKeyboard* GetKeyboard() const = 0;
  CS_FUNCTION()
  CS_NODISCARD virtual cs::iMouse* GetMouse() const = 0;


};

}