#ifndef IKEYBOARD_HH
#define IKEYBOARD_HH

#include <csCore/csCoreExport.hh>
#include <csCore/input/eKey.hh>
#include <csCore/csClass.hh>


namespace cs
{

CS_CLASS(jclass = "org.cryo.core.input.IKeyboard")
struct CS_CORE_API iKeyboard : public CS_SUPER(iObject)
{
CS_CLASS_GEN;
  ~iKeyboard() override = default;

  CS_FUNCTION(jenum="org.cryo.core.input.EKey")
  virtual bool IsKeyDown(cs::eKey key) const = 0;
  CS_FUNCTION(jenum="org.cryo.core.input.EKey")
  virtual bool IsKeyUp(cs::eKey key) const = 0;

  CS_FUNCTION(jenum="org.cryo.core.input.EKey")
  virtual bool IsKeyPressed(cs::eKey key) const = 0;
  CS_FUNCTION(jenum="org.cryo.core.input.EKey")
  virtual bool IsKeyReleased(cs::eKey key) const = 0;

};

}

#endif // IKEYBOARD_HH
