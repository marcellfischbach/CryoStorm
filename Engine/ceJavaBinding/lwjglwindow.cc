//
// Created by MCEL on 08.05.2024.
//

#include <ceJavaBinding/lwjglwindow.hh>
#include <ceCore/java.hh>
#include <ceCore/java_methods.hh>

#define THIS_CLASS_NAME "org/crimsonedge/core/window/IWindow"

extern "C"
{

JNIEXPORT jlong JNICALL Java_org_crimsonedge_launcher_LwjglWindow_nCreate(JNIEnv *env, jclass cls)
{
  ce::Java::Set(env);
  auto lwjgWindow = new ce::java::LwjglWindow();
  return reinterpret_cast<jlong>(lwjgWindow);
}


JNIEXPORT void JNICALL Java_org_crimsonedge_launcher_LwjglWindow_nSetJObject(JNIEnv *env, jclass cls, jlong ref, jobject object)
{
  object = env->NewGlobalRef(object);

  auto window = reinterpret_cast<ce::java::LwjglWindow*>(ref);
  window->SetJObject(object);

}

JNIEXPORT jobject JNICALL Java_org_crimsonedge_launcher_LwjglWindow_nGetMouse(JNIEnv *env, jclass cls, jlong ref)
{
  auto window = reinterpret_cast<ce::java::LwjglWindow*>(ref);
  return window->GetMouse()->GetJObject();
}


JNIEXPORT jobject JNICALL Java_org_crimsonedge_launcher_LwjglWindow_nGetKeyboard(JNIEnv *env, jclass cls, jlong ref)
{
  auto window = reinterpret_cast<ce::java::LwjglWindow*>(ref);
  return window->GetKeyboard()->GetJObject();
}

}

namespace ce::java
{

LwjglWindow *LwjglWindow::s_instance = nullptr;

LwjglWindow *LwjglWindow::Get()
{
  return s_instance;
}

LwjglWindow::LwjglWindow()
{
  s_instance = this;
}

void LwjglWindow::SetTitle(const std::string &title)
{
  static JavaCallVoid1<jstring> setTitle(Java::Get(), this, THIS_CLASS_NAME, "setTitle", JAVA_STRING);

  jstring titleString = Java::Get()->NewStringUTF(title.c_str());
  setTitle.call(Java::Get(), titleString);

}

const std::string &LwjglWindow::GetTitle() const
{
  static JavaCallObject<jstring> getTitle(Java::Get(), this, THIS_CLASS_NAME, "getTitle", JAVA_STRING);

  jstring jString = getTitle.call(Java::Get(), nullptr);
  if (jString)
  {
    const char* ptr = Java::Get()->GetStringUTFChars(jString, 0);
    m_title = std::string(ptr);
    Java::Get()->ReleaseStringUTFChars(jString, ptr);
  }
  return m_title;
}

void LwjglWindow::SetPosition(int16_t x, int16_t y)
{
  static JavaCallVoid2<jint, jint> setPosition (Java::Get(), this, THIS_CLASS_NAME, "setPosition", JAVA_INT, JAVA_INT);
  setPosition.call(Java::Get(), x, y);
}

int LwjglWindow::GetPositionX() const
{
  static JavaCallInt getPositionX (Java::Get(), this, THIS_CLASS_NAME, "getPositionX");

  return (int)getPositionX.call(Java::Get(), 0);
}
int LwjglWindow::GetPositionY() const
{
  static JavaCallInt getPositionY (Java::Get(), this, THIS_CLASS_NAME, "getPositionY");

  return (int)getPositionY.call(Java::Get(), 0);
}

void LwjglWindow::SetResolution(uint16_t width, uint16_t height)
{
  static JavaCallVoid2<jint, jint> setResolution (Java::Get(), this, THIS_CLASS_NAME, "setResoluation", JAVA_INT, JAVA_INT);
  setResolution.call(Java::Get(), width, height);
}
int LwjglWindow::GetWidth() const
{
  static JavaCallInt getWidth (Java::Get(), this, THIS_CLASS_NAME, "getWidth");
  return getWidth.call(Java::Get(), 100);
}
int LwjglWindow::GetHeight() const
{
  static JavaCallInt getHeight (Java::Get(), this, THIS_CLASS_NAME, "getHeight");
  return getHeight.call(Java::Get(), 100);
}


void LwjglWindow::SetWindowIcon(const ResourceLocator &iconName)
{

}

void LwjglWindow::Show()
{

}
void LwjglWindow::Hide()
{

}

void LwjglWindow::Present()
{

}

void LwjglWindow::ProcessUpdates()
{
}

LwjglMouse *LwjglWindow::GetMouse()
{
  return &m_mouse;
}

LwjglKeyboard *LwjglWindow::GetKeyboard()
{
  return &m_keyboard;
}


}
