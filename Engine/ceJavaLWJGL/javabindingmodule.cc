

#include <jni.h>
#include <master.refl.cc>
#include <ceJavaLWJGL/javabindingmodule.hh>
#include <ceJavaLWJGL/lwjglwindow.hh>
#include <ceJavaLWJGL/lwjglinputsystem.hh>
#include <ceCore/game_java.hh>
#include <ceCore/objectregistry.hh>
#include <ceCore/engine.hh>
#include <ceCore/java.hh>
#include <ceCore/input/iinputsystem.hh>
#include <ceCore/window/iwindow.hh>



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



JNIEXPORT jlong
JNICALL Java_org_crimsonedge_lwjgl_LwjglModule_nCreateLwjglModule(JNIEnv *env, jclass cls)
{
  return reinterpret_cast<jlong>(new ce::java::JavaBindingModule());
}


}