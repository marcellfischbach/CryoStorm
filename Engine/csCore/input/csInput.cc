
#include <csCore/input/csInput.hh>
#include <csCore/input/iInputSystem.hh>
#include <csCore/csObjectRegistry.hh>

namespace cs
{


iInputSystem *csInput::m_cache = nullptr;

iInputSystem *csInput::Get()
{
  if (!m_cache)
  {
    m_cache = csObjectRegistry::Get<iInputSystem>();
  }
  return m_cache;
}

void csInput::Invalidate()
{
  m_cache = nullptr;
}

bool csInput::IsKeyDown(eKey key)
{
  return Get()->GetKeyboard()->IsKeyDown(key);
}

bool csInput::IsKeyUp(eKey key)
{
  return Get()->GetKeyboard()->IsKeyUp(key);
}

bool csInput::IsKeyPressed(eKey key)
{
  return Get()->GetKeyboard()->IsKeyPressed(key);
}

bool csInput::IsKeyReleased(eKey key)
{
  return Get()->GetKeyboard()->IsKeyReleased(key);
}

csVector2f csInput::GetMousePosition()
{
  const iMouse *mouse = Get()->GetMouse();
  return csVector2f((float) mouse->GetX(), (float) mouse->GetY());
}

csVector2f csInput::GetMouseDelta()
{
  const iMouse *mouse = Get()->GetMouse();
  return csVector2f((float) mouse->GetDeltaX(), (float) mouse->GetDeltaY());
}

csVector2f csInput::GetWheel()
{
  const iMouse *mouse = Get()->GetMouse();
  return csVector2f((float) mouse->GetWheelHorizontal(), (float) mouse->GetWheel());
}


bool csInput::IsButtonDown(eMouseButton mouseButton)
{
  return Get()->GetMouse()->IsButtonDown(mouseButton);
}

bool csInput::IsButtonUp(eMouseButton mouseButton)
{
  return Get()->GetMouse()->IsButtonUp(mouseButton);
}

bool csInput::IsButtonPressed(eMouseButton mouseButton)
{
  return Get()->GetMouse()->IsButtonPressed(mouseButton);
}

bool csInput::IsButtonReleased(eMouseButton mouseButton)
{
  return Get()->GetMouse()->IsButtonReleased(mouseButton);
}

iMouse *csInput::GetMouse()
{
  return Get()->GetMouse();
}

iKeyboard *csInput::GetKeyboard()
{
  return Get()->GetKeyboard();
}


}


#ifdef CS_JAVA

extern "C"
{

JNIEXPORT void
JNICALL Java_org_cryo_core_input_CsInput_nInvalidate(JNIEnv *env, jclass)
{
  cs::csInput::Invalidate();
}


JNIEXPORT jobject
JNICALL Java_org_cryo_core_input_CsInput_nGetKeyboard(JNIEnv *env, jclass)
{
  cs::iKeyboard *pKeyboard = cs::csInput::GetKeyboard();
  return pKeyboard ? pKeyboard->GetJObject() : nullptr;
}


JNIEXPORT jobject
JNICALL Java_org_cryo_core_input_CsInput_nGetMouse(JNIEnv *env, jclass)
{
  cs::iMouse *pMouse = cs::csInput::GetMouse();
  return pMouse ? pMouse->GetJObject() : nullptr;
}

}

#endif