

#include <jni.h>
#include <master.refl.cc>
#include <ceJavaBinding/javabindingmodule.hh>
#include <ceCore/game_java.hh>
#include <ceJavaBinding/lwjglwindow.hh>
#include <ceJavaBinding/lwjglinputsystem.hh>
#include <ceCore/objectregistry.hh>
#include <ceCore/engine.hh>
#include <ceCore/java.hh>
#include <ceCore/input/iinputsystem.hh>
#include <ceCore/window/iwindow.hh>

extern "C"
{


JNIEXPORT void JNICALL Java_org_crimsonedge_launcher_JavaBinding_nSetEnv(JNIEnv *env, jclass cls)
{
  ce::Java::Set(env);
}


}


namespace ce::java
{

bool JavaBindingModule::Register(const std::vector<std::string> &args, Engine *engine)
{
  register_classes();

//  SDLWindow *window = new SDLWindow();
  LwjglWindow      *window      = LwjglWindow::Get();
  LwjglInputSystem *inputSystem = new LwjglInputSystem(window->GetKeyboard(), window->GetMouse());
  ObjectRegistry::Register<iInputSystem>(inputSystem);
  ObjectRegistry::Register<iWindow>(window);
  engine->SetWindow(window);

  return true;
}

bool JavaBindingModule::Initialize(const std::vector<std::string> &args, Engine *engine)
{
//  SDLWindow *window = (SDLWindow*)engine->GetWindow();
//  window->Initialize();
//
//  const std::string &iconName = Settings::Get().Display().GetText("icon");
//  if (!iconName.empty())
//  {
//    window->SetWindowIcon(ResourceLocator(iconName));
//  }
//
//  window->Show();
  return true;
}


}


extern "C"
{


JNIEXPORT jboolean
JNICALL Java_org_crimsonedge_core_Engine_nInitialize(JNIEnv *env, jclass cls, jobjectArray jargs, jobject gameObject)
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

  bool result = ce::Engine::Get()->Initialize(args, new ce::java::JavaBindingModule(), game);
  fflush(stdout);
  return result;
}

extern "C" JNIEXPORT void JNICALL Java_org_crimsonedge_core_Engine_nProcessFrame(JNIEnv *env, jclass cls)
{
  ce::Java::Set(env);
  ce::Engine::Get()->ProcessFrame();
}


}