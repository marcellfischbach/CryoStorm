//
// Created by MCEL on 08.05.2024.
//

#include <csJavaLWJGL/csLwjglMouse.hh>
#include <csCore/csJavaCalls.hh>

#define THIS_CLASS "org/crimsonedge/lwjgl/LwjglMouse"

namespace cryo::java
{

csLwjglMouse::csLwjglMouse()
{
  JNIEnv *pEnv = csJava::Get();
  cls = pEnv->FindClass(THIS_CLASS);
  jmethodID constructor = pEnv->GetMethodID(cls, "<init>", "(J)V");
  jobj = pEnv->NewObject(cls, constructor, reinterpret_cast<jlong>(this));
  jobj = pEnv->NewGlobalRef(jobj);
}

jobject  csLwjglMouse::GetJObject()
{
  return jobj;
}

void csLwjglMouse::SetVisible(bool visible)
{
  static csJavaCallVoid1<jboolean> setVisible(csJava::Get(), jobj, THIS_CLASS, "setVisible", JAVA_BOOLEAN);
  setVisible.call(csJava::Get(), visible);
}

bool csLwjglMouse::IsVisible() const
{
  static csJavaCallBoolean isVisible(csJava::Get(), jobj, THIS_CLASS, "isVisible");
  return isVisible.call(csJava::Get(), false);
}

void csLwjglMouse::SetCursorMode(eCursorMode mode)
{
  static csJavaCallVoid1<jint> setCursorMode(csJava::Get(), jobj, THIS_CLASS, "setCursorMode", JAVA_INT);
  setCursorMode.call(csJava::Get(), mode);
}

eCursorMode csLwjglMouse::GetCursorMode() const
{
  static csJavaCallInt getCursorMode(csJava::Get(), jobj, THIS_CLASS, "getCursorMode");
  return (eCursorMode)getCursorMode.call(csJava::Get(), 0);
}


int32_t csLwjglMouse::GetX() const
{
  static csJavaCallInt getX(csJava::Get(), jobj, THIS_CLASS, "getX");
  return getX.call(csJava::Get(), 0);
}

int32_t csLwjglMouse::GetY() const
{
  static csJavaCallInt getY(csJava::Get(), jobj, THIS_CLASS, "getY");
  return getY.call(csJava::Get(), 0);
}

int32_t csLwjglMouse::GetDeltaX() const
{
  static csJavaCallInt getDeltaX(csJava::Get(), jobj, THIS_CLASS, "getDeltaX");
  jint                 deltaX = getDeltaX.call(csJava::Get(), 0);
  return deltaX;
}

int32_t csLwjglMouse::GetDeltaY() const
{
  static csJavaCallInt getDeltaY(csJava::Get(), jobj, THIS_CLASS, "getDeltaY");
  jint                 deltaY = getDeltaY.call(csJava::Get(), 0);
  return deltaY;
}

int32_t csLwjglMouse::GetWheel() const
{
  static csJavaCallInt getWheel(csJava::Get(), jobj, THIS_CLASS, "getWheel");
  return getWheel.call(csJava::Get(), 0);
}
int32_t csLwjglMouse::GetWheelHorizontal() const
{
  static csJavaCallInt getWheelHorizontal(csJava::Get(), jobj, THIS_CLASS, "getWheelHorizontal");
  return getWheelHorizontal.call(csJava::Get(), 0);
}

bool csLwjglMouse::IsButtonDown(eMouseButton mouseButton) const
{
  static csJavaCallBoolean1<jint> isButtonDown(csJava::Get(), jobj, THIS_CLASS, "isButtonDown", JAVA_INT);
  return isButtonDown.call(csJava::Get(), mouseButton, false);
}

bool csLwjglMouse::IsButtonUp(eMouseButton mouseButton) const
{
  static csJavaCallBoolean1<jint> isButtonUp(csJava::Get(), jobj, THIS_CLASS, "isButtonUp", JAVA_INT);
  return isButtonUp.call(csJava::Get(), mouseButton, true);
}
bool csLwjglMouse::IsButtonPressed(eMouseButton mouseButton) const
{
  static csJavaCallBoolean1<jint> isButtonPressed(csJava::Get(), jobj, THIS_CLASS, "isButtonPressed", JAVA_INT);
  return isButtonPressed.call(csJava::Get(), mouseButton, false);
}
bool csLwjglMouse::IsButtonReleased(eMouseButton mouseButton) const
{
  static csJavaCallBoolean1<jint> isButtonReleased(csJava::Get(), jobj, THIS_CLASS, "isButtonReleased", JAVA_INT);
  return isButtonReleased.call(csJava::Get(), mouseButton, false);
}

}