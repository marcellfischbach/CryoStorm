//
// Created by MCEL on 21.03.2025.
//

#include <csCore/input/csJavaMouse.hh>
#include <csCore/csJava.hh>
#include <csCore/csJavaCalls.hh>

#define THIS_JAVA_CLASS "org/cryo/core/input/MouseJava"

namespace cs
{

struct csJavaMousePrivate
{
  csJavaMousePrivate(csJavaMouse *mouse)
  {
    JNIEnv *env = csJava::Get();

    getX               = csJavaCallInt(env, mouse, THIS_JAVA_CLASS, "getX");
    getY               = csJavaCallInt(env, mouse, THIS_JAVA_CLASS, "getY");
    getDeltaX          = csJavaCallInt(env, mouse, THIS_JAVA_CLASS, "getDeltaX");
    getDeltaY          = csJavaCallInt(env, mouse, THIS_JAVA_CLASS, "getDeltaY");
    getWheel           = csJavaCallInt(env, mouse, THIS_JAVA_CLASS, "getWheel");
    getWheelHorizontal = csJavaCallInt(env, mouse, THIS_JAVA_CLASS, "getWheelHorizontal");
    isButtonDown       = csJavaCallBoolean1<jint>(env, mouse, THIS_JAVA_CLASS, "isButtonDown", JAVA_INT);
    isButtonUp         = csJavaCallBoolean1<jint>(env, mouse, THIS_JAVA_CLASS, "isButtonUp", JAVA_INT);
    isButtonPressed    = csJavaCallBoolean1<jint>(env, mouse, THIS_JAVA_CLASS, "isButtonPressed", JAVA_INT);
    isButtonReleased   = csJavaCallBoolean1<jint>(env, mouse, THIS_JAVA_CLASS, "isButtonReleased", JAVA_INT);


  }

  csJavaCallInt            getX;
  csJavaCallInt            getY;
  csJavaCallInt            getDeltaX;
  csJavaCallInt            getDeltaY;
  csJavaCallInt            getWheel;
  csJavaCallInt            getWheelHorizontal;
  csJavaCallBoolean1<jint> isButtonDown;
  csJavaCallBoolean1<jint> isButtonUp;
  csJavaCallBoolean1<jint> isButtonPressed;
  csJavaCallBoolean1<jint> isButtonReleased;

};


csJavaMouse::csJavaMouse()
    : m_priv(new csJavaMousePrivate(this))
{

}

csJavaMouse::~csJavaMouse()
{
  delete m_priv;
  m_priv = nullptr;

}

void csJavaMouse::SetVisible(bool visible)
{
}


bool csJavaMouse::IsVisible() const
{
  return true;
}


void csJavaMouse::SetCursorMode(eCursorMode mode)
{
}


eCursorMode csJavaMouse::GetCursorMode() const
{
  return eCM_Free;
}


int32_t csJavaMouse::GetX() const
{
  return m_priv->getX.call(csJava::Get(), 0);
}


int32_t csJavaMouse::GetY() const
{
  return m_priv->getY.call(csJava::Get(), 0);
}


int32_t csJavaMouse::GetDeltaX() const
{
  return m_priv->getDeltaX.call(csJava::Get(), 0);
}


int32_t csJavaMouse::GetDeltaY() const
{
  return m_priv->getDeltaY.call(csJava::Get(), 0);
}


int32_t csJavaMouse::GetWheel() const
{
  return m_priv->getWheel.call(csJava::Get(), 0);
}


int32_t csJavaMouse::GetWheelHorizontal() const
{
  return m_priv->getWheelHorizontal.call(csJava::Get(), 0);
}


bool csJavaMouse::IsButtonDown(eMouseButton mouseButton) const
{
  return m_priv->isButtonDown.call(csJava::Get(), mouseButton, false);
}


bool csJavaMouse::IsButtonUp(eMouseButton mouseButton) const
{
  return m_priv->isButtonUp.call(csJava::Get(), mouseButton, false);
}


bool csJavaMouse::IsButtonPressed(eMouseButton mouseButton) const
{
  return m_priv->isButtonPressed.call(csJava::Get(), mouseButton, false);
}


bool csJavaMouse::IsButtonReleased(eMouseButton mouseButton) const
{
  return m_priv->isButtonReleased.call(csJava::Get(), mouseButton, false);
}


}