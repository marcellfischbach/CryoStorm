

#include <jni.h>
#include <master.refl.cc>
#include <ceJavaLWJGL/csJavaBindingModule.hh>
#include <ceJavaLWJGL/csLwjglWindow.hh>
#include <ceJavaLWJGL/csLwjglInputSystem.hh>
#include <ceCore/csJavaGame.hh>
#include <ceCore/csObjectRegistry.hh>
#include <ceCore/csEngine.hh>
#include <ceCore/csJava.hh>
#include <ceCore/input/iInputSystem.hh>
#include <ceCore/window/iWindow.hh>



namespace cryo::java
{

bool csJavaBindingModule::Register(const std::vector<std::string> &args, csEngine *engine)
{
  register_classes();

//  SDLWindow *window = new SDLWindow();
  csLwjglWindow      *window      = csLwjglWindow::Get();
  csLwjglInputSystem *inputSystem = new csLwjglInputSystem(window->GetKeyboard(), window->GetMouse());
  csObjectRegistry::Register<iInputSystem>(inputSystem);
  csObjectRegistry::Register<iWindow>(window);
  engine->SetWindow(window);

  return true;
}

bool csJavaBindingModule::Initialize(const std::vector<std::string> &args, csEngine *engine)
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
  return reinterpret_cast<jlong>(new cryo::java::csJavaBindingModule());
}


}