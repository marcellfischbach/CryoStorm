//
// Created by MCEL on 08.05.2024.
//

#include <csJavaLWJGL/csLwjglWindow.hh>
#include <csCore/csJava.hh>
#include <csCore/csJavaCalls.hh>

#define THIS_CLASS_NAME "org/crimsonedge/core/window/IWindow"

extern "C"
{

JNIEXPORT jlong JNICALL Java_org_crimsonedge_lwjgl_LwjglWindow_nCreate(JNIEnv *env, jclass cls)
{
  cs::csJava::Set(env);
  auto lwjgWindow = new cs::java::csLwjglWindow();
  return reinterpret_cast<jlong>(lwjgWindow);
}


JNIEXPORT void JNICALL Java_org_crimsonedge_lwjgl_LwjglWindow_nSetJObject(JNIEnv *env, jclass cls, jlong ref, jobject object)
{
  object = env->NewGlobalRef(object);

  auto window = reinterpret_cast<cs::java::csLwjglWindow*>(ref);
  window->SetJObject(object);

}

JNIEXPORT jobject JNICALL Java_org_crimsonedge_lwjgl_LwjglWindow_nGetMouse(JNIEnv *env, jclass cls, jlong ref)
{
  auto window = reinterpret_cast<cs::java::csLwjglWindow*>(ref);
  return window->GetMouse()->GetJObject();
}


JNIEXPORT jobject JNICALL Java_org_crimsonedge_lwjgl_LwjglWindow_nGetKeyboard(JNIEnv *env, jclass cls, jlong ref)
{
  auto window = reinterpret_cast<cs::java::csLwjglWindow*>(ref);
  return window->GetKeyboard()->GetJObject();
}

}

namespace cs::java
{

csLwjglWindow *csLwjglWindow::s_instance = nullptr;

csLwjglWindow *csLwjglWindow::Get()
{
  return s_instance;
}

csLwjglWindow::csLwjglWindow()
{
  s_instance = this;
}

void csLwjglWindow::SetTitle(const std::string &title)
{
  static csJavaCallVoid1<jstring> setTitle(csJava::Get(), this, THIS_CLASS_NAME, "setTitle", JAVA_STRING);

  jstring titleString = csJava::Get()->NewStringUTF(title.c_str());
  setTitle.call(csJava::Get(), titleString);

}

const std::string &csLwjglWindow::GetTitle() const
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

void csLwjglWindow::SetPosition(int16_t x, int16_t y)
{
  static csJavaCallVoid2<jint, jint> setPosition (csJava::Get(), this, THIS_CLASS_NAME, "setPosition", JAVA_INT, JAVA_INT);
  setPosition.call(csJava::Get(), x, y);
}

int16_t csLwjglWindow::GetPositionX() const
{
  static csJavaCallInt getPositionX (csJava::Get(), this, THIS_CLASS_NAME, "getPositionX");

  return (int)getPositionX.call(csJava::Get(), 0);
}
int16_t csLwjglWindow::GetPositionY() const
{
  static csJavaCallInt getPositionY (csJava::Get(), this, THIS_CLASS_NAME, "getPositionY");

  return (int)getPositionY.call(csJava::Get(), 0);
}

void csLwjglWindow::SetResolution(uint16_t width, uint16_t height)
{
  static csJavaCallVoid2<jint, jint> setResolution (csJava::Get(), this, THIS_CLASS_NAME, "setResoluation", JAVA_INT, JAVA_INT);
  setResolution.call(csJava::Get(), width, height);
}
int16_t csLwjglWindow::GetWidth() const
{
  static csJavaCallInt getWidth (csJava::Get(), this, THIS_CLASS_NAME, "getWidth");
  return getWidth.call(csJava::Get(), 100);
}
int16_t csLwjglWindow::GetHeight() const
{
  static csJavaCallInt getHeight (csJava::Get(), this, THIS_CLASS_NAME, "getHeight");
  return getHeight.call(csJava::Get(), 100);
}


void csLwjglWindow::SetWindowIcon(const csResourceLocator &iconName)
{

}

void csLwjglWindow::Show()
{

}
void csLwjglWindow::Hide()
{

}

void csLwjglWindow::Present()
{

}

void csLwjglWindow::ProcessUpdates()
{
}

csLwjglMouse *csLwjglWindow::GetMouse()
{
  return &m_mouse;
}

csLwjglKeyboard *csLwjglWindow::GetKeyboard()
{
  return &m_keyboard;
}


}
