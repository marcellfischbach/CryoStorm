//
// Created by MCEL on 08.05.2024.
//

#include <csJavaLWJGL/csLwjglKeyboard.hh>
#include <csCore/csJavaCalls.hh>

#define THIS_CLASS "org/crimsonedge/lwjgl/LwjglKeyboard"

namespace cs::java
{
csLwjglKeyboard::csLwjglKeyboard()
{
  JNIEnv *pEnv = csJava::Get();
  cls = pEnv->FindClass(THIS_CLASS);
  jmethodID constructor = pEnv->GetMethodID(cls, "<init>", "(J)V");
  jobj = pEnv->NewObject(cls, constructor, reinterpret_cast<jlong>(this));
  jobj = pEnv->NewGlobalRef(jobj);
}

jobject csLwjglKeyboard::GetJObject()
{
  return jobj;
}

bool csLwjglKeyboard::IsKeyDown(cs::eKey key) const
{
  static csJavaCallBoolean1<jint> jcall (csJava::Get(), jobj, THIS_CLASS, "isKeyDown", JAVA_INT);
  return jcall.call(csJava::Get(), key, false);
}

bool csLwjglKeyboard::IsKeyUp(cs::eKey key) const
{
  static csJavaCallBoolean1<jint> jcall (csJava::Get(), jobj, THIS_CLASS, "isKeyUp", JAVA_INT);
  return jcall.call(csJava::Get(), key, true);
}


bool csLwjglKeyboard::IsKeyPressed(cs::eKey key) const
{
  static csJavaCallBoolean1<jint> jcall (csJava::Get(), jobj, THIS_CLASS, "isKeyPressed", JAVA_INT);
  return jcall.call(csJava::Get(), key, false);
}

bool csLwjglKeyboard::IsKeyReleased(cs::eKey key) const
{
  static csJavaCallBoolean1<jint> jcall (csJava::Get(), jobj, THIS_CLASS, "isKeyReleased", JAVA_INT);
  return jcall.call(csJava::Get(), key, false);
}


}
