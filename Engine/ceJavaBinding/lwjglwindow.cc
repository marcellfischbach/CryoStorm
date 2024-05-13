//
// Created by MCEL on 08.05.2024.
//

#include <ceJavaBinding/lwjglwindow.hh>
#include <ceCore/java.hh>

extern "C"
{
JNIEXPORT jlong JNICALL Java_org_crimsonedge_launcher_LwjglWindow_nInitWindow(JNIEnv *env, jclass cls, jobject window)
{
  window = env->NewGlobalRef(window);

  ce::java::LwjglWindow *lwjgWindow = new ce::java::LwjglWindow(window);
  return reinterpret_cast<jlong>(lwjgWindow);
}

}

namespace ce::java
{

LwjglWindow *LwjglWindow::s_instance = nullptr;

LwjglWindow *LwjglWindow::Get()
{
  return s_instance;
}

LwjglWindow::LwjglWindow(jobject javaWindow)
    : m_javaWindow(javaWindow)
{
  s_instance = this;
}

void LwjglWindow::SetTitle(const std::string &title)
{
  // dont set from outside
}

const std::string &LwjglWindow::GetTitle() const
{
  return m_title;
}

void LwjglWindow::SetPosition(int16_t x, int16_t y)
{
  // dont set from outside
}

int LwjglWindow::GetPositionX() const
{

  return 0;
}
int LwjglWindow::GetPositionY() const
{
  return 0;
}

void LwjglWindow::SetResolution(uint16_t width, uint16_t height)
{

}
int LwjglWindow::GetWidth() const
{
  JNIEnv           *env    = Java::Get();
  static jclass    cls            = env->FindClass("org/crimsonedge/launcher/LwjglWindow");
  static jmethodID getWindowWidth = env->GetMethodID(cls, "getWindowWidth", "()I");
  return env->CallIntMethod(m_javaWindow, getWindowWidth);
}
int LwjglWindow::GetHeight() const
{
  JNIEnv           *env    = Java::Get();
  static jclass    cls            = env->FindClass("org/crimsonedge/launcher/LwjglWindow");
  static jmethodID getWindowHeight = env->GetMethodID(cls, "getWindowHeight", "()I");
  return env->CallIntMethod(m_javaWindow, getWindowHeight);
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
  m_keyboard.Swap();
}

LwjglMouse* LwjglWindow::GetMouse()
{
  return &m_mouse;
}

LwjglKeyboard* LwjglWindow::GetKeyboard()
{
  return &m_keyboard;
}



}
