//
// Created by MCEL on 21.03.2025.
//

#include <csCore/input/csJavaKeyboard.hh>
#include <csCore/csJava.hh>
#include <csCore/csJavaCalls.hh>


#define THIS_JAVA_CLASS "org/cryo/core/input/KeyboardJava"

namespace cs
{

struct csJavaKeyboardPrivate
{
  csJavaKeyboardPrivate (csJavaKeyboard *that)
  {
    JNIEnv *env = csJava::Get();
    isKeyDown = csJavaCallBoolean1<jint>(env, that, THIS_JAVA_CLASS, "isKeyDown", JAVA_INT);
    isKeyUp = csJavaCallBoolean1<jint>(env, that, THIS_JAVA_CLASS, "isKeyUp", JAVA_INT);
    isKeyPressed = csJavaCallBoolean1<jint>(env, that, THIS_JAVA_CLASS, "isKeyPressed", JAVA_INT);
    isKeyReleased = csJavaCallBoolean1<jint>(env, that, THIS_JAVA_CLASS, "isKeyReleased", JAVA_INT);
  }
  csJavaCallBoolean1<jint> isKeyDown;
  csJavaCallBoolean1<jint> isKeyUp;
  csJavaCallBoolean1<jint> isKeyPressed;
  csJavaCallBoolean1<jint> isKeyReleased;
};


csJavaKeyboard::csJavaKeyboard()
: iKeyboard()
, m_priv(new csJavaKeyboardPrivate(this))
{
  
}

csJavaKeyboard::~csJavaKeyboard ()
{
  delete m_priv;
  m_priv = nullptr;
}

bool csJavaKeyboard::IsKeyDown(eKey key) const
{
  return m_priv->isKeyDown.call(csJava::Get(), key, false);
}

bool csJavaKeyboard::IsKeyUp(eKey key) const 
{
  return m_priv->isKeyUp.call(csJava::Get(), key, false);
}

bool csJavaKeyboard::IsKeyPressed(eKey key) const
{
  return m_priv->isKeyPressed.call(csJava::Get(), key, false);
}

bool csJavaKeyboard::IsKeyReleased(eKey key) const
{
  return m_priv->isKeyReleased.call(csJava::Get(), key, false);
}


} // cs