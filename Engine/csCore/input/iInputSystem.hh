
#pragma once


#include <csCore/csCoreExport.hh>
#include <csCore/csDefs.hh>
#include <csCore/input/iKeyboard.hh>
#include <csCore/input/iMouse.hh>
#include <csCore/csClass.hh>


namespace cs
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