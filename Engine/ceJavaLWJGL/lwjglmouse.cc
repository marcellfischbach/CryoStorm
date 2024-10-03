//
// Created by MCEL on 08.05.2024.
//

#include <ceJavaLWJGL/lwjglmouse.hh>
#include <ceCore/java_methods.hh>

#define THIS_CLASS "org/crimsonedge/lwjgl/LwjglMouse"

namespace cryo::java
{

LwjglMouse::LwjglMouse()
{
  JNIEnv *pEnv = Java::Get();
  cls = pEnv->FindClass(THIS_CLASS);
  jmethodID constructor = pEnv->GetMethodID(cls, "<init>", "(J)V");
  jobj = pEnv->NewObject(cls, constructor, reinterpret_cast<jlong>(this));
  jobj = pEnv->NewGlobalRef(jobj);
}

jobject  LwjglMouse::GetJObject()
{
  return jobj;
}

void LwjglMouse::SetVisible(bool visible)
{
  static JavaCallVoid1<jboolean> setVisible(Java::Get(), jobj, THIS_CLASS, "setVisible", JAVA_BOOLEAN);
  setVisible.call(Java::Get(), visible);
}

bool LwjglMouse::IsVisible() const
{
  static JavaCallBoolean isVisible(Java::Get(), jobj, THIS_CLASS, "isVisible");
  return isVisible.call(Java::Get(), false);
}

void LwjglMouse::SetCursorMode(eCursorMode mode)
{
  static JavaCallVoid1<jint> setCursorMode(Java::Get(), jobj, THIS_CLASS, "setCursorMode", JAVA_INT);
  setCursorMode.call(Java::Get(), mode);
}

eCursorMode LwjglMouse::GetCursorMode() const
{
  static JavaCallInt getCursorMode(Java::Get(), jobj, THIS_CLASS, "getCursorMode");
  return (eCursorMode)getCursorMode.call(Java::Get(), 0);
}


int32_t LwjglMouse::GetX() const
{
  static JavaCallInt getX(Java::Get(), jobj, THIS_CLASS, "getX");
  return getX.call(Java::Get(), 0);
}

int32_t LwjglMouse::GetY() const
{
  static JavaCallInt getY(Java::Get(), jobj, THIS_CLASS, "getY");
  return getY.call(Java::Get(), 0);
}

int32_t LwjglMouse::GetDeltaX() const
{
  static JavaCallInt getDeltaX(Java::Get(), jobj, THIS_CLASS, "getDeltaX");
  jint deltaX = getDeltaX.call(Java::Get(), 0);
  return deltaX;
}

int32_t LwjglMouse::GetDeltaY() const
{
  static JavaCallInt getDeltaY(Java::Get(), jobj, THIS_CLASS, "getDeltaY");
  jint deltaY = getDeltaY.call(Java::Get(), 0);
  return deltaY;
}

int32_t LwjglMouse::GetWheel() const
{
  static JavaCallInt getWheel(Java::Get(), jobj, THIS_CLASS, "getWheel");
  return getWheel.call(Java::Get(), 0);
}
int32_t LwjglMouse::GetWheelHorizontal() const
{
  static JavaCallInt getWheelHorizontal(Java::Get(), jobj, THIS_CLASS, "getWheelHorizontal");
  return getWheelHorizontal.call(Java::Get(), 0);
}

bool LwjglMouse::IsButtonDown(eMouseButton mouseButton) const
{
  static JavaCallBoolean1<jint> isButtonDown(Java::Get(), jobj, THIS_CLASS, "isButtonDown", JAVA_INT);
  return isButtonDown.call(Java::Get(), mouseButton, false);
}

bool LwjglMouse::IsButtonUp(eMouseButton mouseButton) const
{
  static JavaCallBoolean1<jint> isButtonUp(Java::Get(), jobj, THIS_CLASS, "isButtonUp", JAVA_INT);
  return isButtonUp.call(Java::Get(), mouseButton, true);
}
bool LwjglMouse::IsButtonPressed(eMouseButton mouseButton) const
{
  static JavaCallBoolean1<jint> isButtonPressed(Java::Get(), jobj, THIS_CLASS, "isButtonPressed", JAVA_INT);
  return isButtonPressed.call(Java::Get(), mouseButton, false);
}
bool LwjglMouse::IsButtonReleased(eMouseButton mouseButton) const
{
  static JavaCallBoolean1<jint> isButtonReleased(Java::Get(), jobj, THIS_CLASS, "isButtonReleased", JAVA_INT);
  return isButtonReleased.call(Java::Get(), mouseButton, false);
}

}