//
// Created by MCEL on 08.05.2024.
//

#include <ceJavaLWJGL/lwjglkeyboard.hh>
#include <ceCore/java_methods.hh>

#define THIS_CLASS "org/crimsonedge/lwjgl/LwjglKeyboard"

namespace cryo::java
{
LwjglKeyboard::LwjglKeyboard()
{
  JNIEnv *pEnv = Java::Get();
  cls = pEnv->FindClass(THIS_CLASS);
  jmethodID constructor = pEnv->GetMethodID(cls, "<init>", "(J)V");
  jobj = pEnv->NewObject(cls, constructor, reinterpret_cast<jlong>(this));
  jobj = pEnv->NewGlobalRef(jobj);
}

jobject LwjglKeyboard::GetJObject()
{
  return jobj;
}

bool LwjglKeyboard::IsKeyDown(cryo::Key key) const
{
  static JavaCallBoolean1<jint> jcall (Java::Get(), jobj, THIS_CLASS, "isKeyDown", JAVA_INT);
  return jcall.call(Java::Get(), key, false);
}

bool LwjglKeyboard::IsKeyUp(cryo::Key key) const
{
  static JavaCallBoolean1<jint> jcall (Java::Get(), jobj, THIS_CLASS, "isKeyUp", JAVA_INT);
  return jcall.call(Java::Get(), key, true);
}


bool LwjglKeyboard::IsKeyPressed(cryo::Key key) const
{
  static JavaCallBoolean1<jint> jcall (Java::Get(), jobj, THIS_CLASS, "isKeyPressed", JAVA_INT);
  return jcall.call(Java::Get(), key, false);
}

bool LwjglKeyboard::IsKeyReleased(cryo::Key key) const
{
  static JavaCallBoolean1<jint> jcall (Java::Get(), jobj, THIS_CLASS, "isKeyReleased", JAVA_INT);
  return jcall.call(Java::Get(), key, false);
}


}
