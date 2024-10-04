
#include <csCore/csEngine.hh>
#include <csCore/csJavaGame.hh>
#include <csCore/csJava.hh>
#include <csCore/entity/csWorld.hh>
#include <csCore/graphics/iDevice.hh>


extern "C"
{


JNIEXPORT void JNICALL Java_org_crimsonedge_core_Engine_nSetEnv(JNIEnv *env, jclass cls)
{
  cryo::csJava::Set(env);
}




JNIEXPORT jobject
JNICALL Java_org_crimsonedge_core_Engine_nGetWorld(JNIEnv *env, jclass cls)
{
  return cryo::csEngine::Get()->GetWorld()->GetJObject();
}


JNIEXPORT jobject
JNICALL Java_org_crimsonedge_core_Engine_nGetDevice(JNIEnv *env, jclass cls)
{
  return cryo::csEngine::Get()->GetDevice()->GetJObject();
}


JNIEXPORT jboolean
JNICALL Java_org_crimsonedge_core_Engine_nInitialize(JNIEnv *env, jclass cls, jobjectArray jargs, jlong externalModuleRef, jobject gameObject)
{
  cryo::csJava::Set(env);
  jsize                    argc = env->GetArrayLength(jargs);
  std::vector<std::string> args;
  for (jsize               i    = 0; i < argc; i++)
  {
    auto       arg   = (jstring) (env->GetObjectArrayElement(jargs, i));
    const char *argv = env->GetStringUTFChars(arg, 0);
    std::string cArg (argv);
    args.push_back(cArg);
    env->ReleaseStringUTFChars(arg, argv);

  }

  gameObject = gameObject ? env->NewGlobalRef(gameObject) : nullptr;

  cryo::csGameJava *game = gameObject
                       ? new cryo::csGameJava(gameObject)
                       : nullptr;

  auto externalModule = reinterpret_cast<cryo::iModule*>(externalModuleRef);

  bool result = cryo::csEngine::Get()->InitializeEngine(args, externalModule);
  fflush(stdout);
  return result;
}

extern "C" JNIEXPORT void JNICALL Java_org_crimsonedge_core_Engine_nProcessFrame(JNIEnv *env, jclass cls)
{
  cryo::csJava::Set(env);
  cryo::csEngine::Get()->ProcessFrame();
}
}

