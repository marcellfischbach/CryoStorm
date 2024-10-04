//
// Created by MCEL on 08.05.2024.
//

#include <ceJavaLWJGL/lwjglwindow.hh>
#include <ceCore/csJava.hh>
#include <ceCore/csJavaCalls.hh>

#define THIS_CLASS_NAME "org/crimsonedge/core/window/IWindow"

extern "C"
{

JNIEXPORT jlong JNICALL Java_org_crimsonedge_lwjgl_LwjglWindow_nCreate(JNIEnv *env, jclass cls)
{
  cryo::csJava::Set(env);
  auto lwjgWindow = new cryo::java::LwjglWindow();
  return reinterpret_cast<jlong>(lwjgWindow);
}


JNIEXPORT void JNICALL Java_org_crimsonedge_lwjgl_LwjglWindow_nSetJObject(JNIEnv *env, jclass cls, jlong ref, jobject object)
{
  object = env->NewGlobalRef(object);

  auto window = reinterpret_cast<cryo::java::LwjglWindow*>(ref);
  window->SetJObject(object);

}

JNIEXPORT jobject JNICALL Java_org_crimsonedge_lwjgl_LwjglWindow_nGetMouse(JNIEnv *env, jclass cls, jlong ref)
{
  auto window = reinterpret_cast<cryo::java::LwjglWindow*>(ref);
  return window->GetMouse()->GetJObject();
}


JNIEXPORT jobject JNICALL Java_org_crimsonedge_lwjgl_LwjglWindow_nGetKeyboard(JNIEnv *env, jclass cls, jlong ref)
{
  auto window = reinterpret_cast<cryo::java::LwjglWindow*>(ref);
  return window->GetKeyboard()->GetJObject();
}

}

namespace cryo::java
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
  static csJavaCallVoid1<jstring> setTitle(csJava::Get(), this, THIS_CLASS_NAME, "setTitle", JAVA_STRING);

  jstring titleString = csJava::Get()->NewStringUTF(title.c_str());
  setTitle.call(csJava::Get(), titleString);

}

const std::string &LwjglWindow::GetTitle() const
{
  static csJavaCallObject<jstring> getTitle(csJava::Get(), this, THIS_CLASS_NAME, "getTitle", JAVA_STRING);

  jstring jString = getTitle.call(csJava::Get(), nullptr);
  if (jString)
  {
    const char* ptr = csJava::Get()->GetStringUTFChars(jString, 0);
    m_title = std::string(ptr);
    csJava::Get()->ReleaseStringUTFChars(jString, ptr);
  }
  return m_title;
}

void LwjglWindow::SetPosition(int16_t x, int16_t y)
{
  static csJavaCallVoid2<jint, jint> setPosition (csJava::Get(), this, THIS_CLASS_NAME, "setPosition", JAVA_INT, JAVA_INT);
  setPosition.call(csJava::Get(), x, y);
}

int LwjglWindow::GetPositionX() const
{
  static csJavaCallInt getPositionX (csJava::Get(), this, THIS_CLASS_NAME, "getPositionX");

  return (int)getPositionX.call(csJava::Get(), 0);
}
int LwjglWindow::GetPositionY() const
{
  static csJavaCallInt getPositionY (csJava::Get(), this, THIS_CLASS_NAME, "getPositionY");

  return (int)getPositionY.call(csJava::Get(), 0);
}

void LwjglWindow::SetResolution(uint16_t width, uint16_t height)
{
  static csJavaCallVoid2<jint, jint> setResolution (csJava::Get(), this, THIS_CLASS_NAME, "setResoluation", JAVA_INT, JAVA_INT);
  setResolution.call(csJava::Get(), width, height);
}
int LwjglWindow::GetWidth() const
{
  static csJavaCallInt getWidth (csJava::Get(), this, THIS_CLASS_NAME, "getWidth");
  return getWidth.call(csJava::Get(), 100);
}
int LwjglWindow::GetHeight() const
{
  static csJavaCallInt getHeight (csJava::Get(), this, THIS_CLASS_NAME, "getHeight");
  return getHeight.call(csJava::Get(), 100);
}


void LwjglWindow::SetWindowIcon(const csResourceLocator &iconName)
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
