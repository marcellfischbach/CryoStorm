
#include <ceCore/engine.hh>
#include <ceCore/game_java.hh>
#include <ceCore/java.hh>
#include <ceCore/entity/world.hh>
#include <ceCore/graphics/idevice.hh>


extern "C"
{


JNIEXPORT void JNICALL Java_org_crimsonedge_core_Engine_nSetEnv(JNIEnv *env, jclass cls)
{
  ce::Java::Set(env);
}




JNIEXPORT jobject
JNICALL Java_org_crimsonedge_core_Engine_nGetWorld(JNIEnv *env, jclass cls)
{
  return ce::Engine::Get()->GetWorld()->GetJObject();
}


JNIEXPORT jobject
JNICALL Java_org_crimsonedge_core_Engine_nGetDevice(JNIEnv *env, jclass cls)
{
  return ce::Engine::Get()->GetDevice()->GetJObject();
}


JNIEXPORT jboolean
JNICALL Java_org_crimsonedge_core_Engine_nInitialize(JNIEnv *env, jclass cls, jobjectArray jargs, jlong externalModuleRef, jobject gameObject)
{
  ce::Java::Set(env);
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

  ce::GameJava *game = gameObject
                       ? new ce::GameJava(gameObject)
                       : nullptr;

  auto externalModule = reinterpret_cast<ce::iModule*>(externalModuleRef);

  bool result = ce::Engine::Get()->InitializeEngine(args, externalModule);
  fflush(stdout);
  return result;
}

extern "C" JNIEXPORT void JNICALL Java_org_crimsonedge_core_Engine_nProcessFrame(JNIEnv *env, jclass cls)
{
  ce::Java::Set(env);
  ce::Engine::Get()->ProcessFrame();
}
}

