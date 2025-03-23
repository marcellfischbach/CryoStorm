//
// Created by Marcell on 21.10.2024.
//

#include <csCore/csJavaModule.hh>
#include <csCore/csJavaCalls.hh>
#include <csCore/csEngine.hh>

#define THIS_CLASS_NAME "org/cryo/core/CsJavaModule"

namespace cs
{
bool csJavaModule::Register(const std::vector<std::string> &args, csEngine *engine)
{
  printf ("Get to here\n");
  JNIEnv *env = csJava::Get();

  jclass stringClass = env->FindClass("java/lang/String");
  jobjectArray stringArray = env->NewObjectArray((jsize) args.size(), stringClass, nullptr);

  jsize idx = 0;
  for (const auto &arg: args)
  {
    jstring string = env->NewStringUTF(arg.c_str());
    env->SetObjectArrayElement(stringArray, idx++, string);
  }



  static csJavaCallBoolean2<jobjectArray, jobject> callRegister (env, this, THIS_CLASS_NAME, "register", "[Ljava/lang/String;", "Lorg/cryo/core/Engine;");
  return callRegister.call(env, stringArray, engine->GetJObject(), false);
}

bool csJavaModule::Initialize(const std::vector<std::string> &args, csEngine *engine)
{
  JNIEnv *env = csJava::Get();

  jclass stringClass = env->FindClass("java/lang/String");
  jobjectArray stringArray = env->NewObjectArray((jsize) args.size(), stringClass, nullptr);

  jsize idx = 0;
  for (const auto &arg: args)
  {
    jstring string = env->NewStringUTF(arg.c_str());
    env->SetObjectArrayElement(stringArray, idx++, string);
  }




  static csJavaCallBoolean2<jobjectArray, jobject> callInitialize (env, this, THIS_CLASS_NAME, "initialize", "[Ljava/lang/String;", "Lorg/cryo/core/Engine;");
  return callInitialize.call(env, stringArray, engine->GetJObject(), false);
}


void csJavaModule::Shutdown(const std::vector<std::string> &args, cs::csEngine *engine)
{

}

} // cs