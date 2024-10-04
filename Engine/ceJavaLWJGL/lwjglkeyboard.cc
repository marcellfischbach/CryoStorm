//
// Created by MCEL on 08.05.2024.
//

#include <ceJavaLWJGL/lwjglkeyboard.hh>
#include <ceCore/csJavaCalls.hh>

#define THIS_CLASS "org/crimsonedge/lwjgl/LwjglKeyboard"

namespace cryo::java
{
LwjglKeyboard::LwjglKeyboard()
{
  JNIEnv *pEnv = csJava::Get();
  cls = pEnv->FindClass(THIS_CLASS);
  jmethodID constructor = pEnv->GetMethodID(cls, "<init>", "(J)V");
  jobj = pEnv->NewObject(cls, constructor, reinterpret_cast<jlong>(this));
  jobj = pEnv->NewGlobalRef(jobj);
}

jobject LwjglKeyboard::GetJObject()
{
  return jobj;
}

bool LwjglKeyboard::IsKeyDown(cryo::eKey key) const
{
  static csJavaCallBoolean1<jint> jcall (csJava::Get(), jobj, THIS_CLASS, "isKeyDown", JAVA_INT);
  return jcall.call(csJava::Get(), key, false);
}

bool LwjglKeyboard::IsKeyUp(cryo::eKey key) const
{
  static csJavaCallBoolean1<jint> jcall (csJava::Get(), jobj, THIS_CLASS, "isKeyUp", JAVA_INT);
  return jcall.call(csJava::Get(), key, true);
}


bool LwjglKeyboard::IsKeyPressed(cryo::eKey key) const
{
  static csJavaCallBoolean1<jint> jcall (csJava::Get(), jobj, THIS_CLASS, "isKeyPressed", JAVA_INT);
  return jcall.call(csJava::Get(), key, false);
}

bool LwjglKeyboard::IsKeyReleased(cryo::eKey key) const
{
  static csJavaCallBoolean1<jint> jcall (csJava::Get(), jobj, THIS_CLASS, "isKeyReleased", JAVA_INT);
  return jcall.call(csJava::Get(), key, false);
}


}
