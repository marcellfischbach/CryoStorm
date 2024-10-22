//
// Created by Marcell on 22.10.2024.
//

#include <csCore/window/csJavaWindow.hh>
#include <csCore/csJavaCalls.hh>

#define DEF_ARGS env, GetJObject(), "org/cryo/core/window/JavaWindow"

namespace cs
{

void csJavaWindow::SetTitle(const std::string &title)
{
  JNIEnv *env = csJava::Get();
  static csJavaCallVoid1<jstring> setTitle(DEF_ARGS, "setTitle", JAVA_STRING);
  setTitle.call(env, env->NewStringUTF(title.c_str()));
}


const std::string &csJavaWindow::GetTitle() const
{
  JNIEnv *env = csJava::Get();
  static csJavaCallObject<jstring> getTitle (DEF_ARGS, "getTitle", JAVA_STRING);
  jstring title = getTitle.call(env, nullptr);
  if (title)
  {
    const char* ptr = env->GetStringUTFChars(title, nullptr);
    m_title = ptr;
    env->ReleaseStringUTFChars(title, ptr);
  }
  else
  {
    m_title = "";
  }
  return m_title;
}

void csJavaWindow::SetPosition(int16_t x, int16_t y)
{
  JNIEnv *env = csJava::Get();
  static csJavaCallVoid2<jint, jint> setPosition (DEF_ARGS, "setPosition", JAVA_SHORT, JAVA_SHORT);
  setPosition.call(env, x, y);
}
int16_t csJavaWindow::GetPositionX() const
{
  JNIEnv *env = csJava::Get();
  static csJavaCallInt getPositionX (DEF_ARGS, "getPositionX");
  return getPositionX.call(env, 0);
}
int16_t csJavaWindow::GetPositionY() const
{
  JNIEnv *env = csJava::Get();
  static csJavaCallInt getPositionY (DEF_ARGS, "getPositionY");
  return getPositionY.call(env, 0);
}

void csJavaWindow::SetResolution(uint16_t width, uint16_t height)
{
  JNIEnv *env = csJava::Get();
  static csJavaCallVoid2<jint, jint> setResolution (DEF_ARGS, "setResolution", JAVA_INT, JAVA_INT);
  setResolution.call(env, width, height);
}
int16_t csJavaWindow::GetWidth() const
{
  JNIEnv *env = csJava::Get();
  static csJavaCallInt getWidth (DEF_ARGS, "getWidth");
  return getWidth.call(env, 0);

}
int16_t csJavaWindow::GetHeight() const
{
  JNIEnv *env = csJava::Get();
  static csJavaCallInt getHeight (DEF_ARGS, "getHeight");
  return getHeight.call(env, 0);
}


/**
 * Optional.
 * @param iconName
 */
void csJavaWindow::SetWindowIcon(const csResourceLocator &iconName)
{

}

void csJavaWindow::Show()
{
  JNIEnv *env = csJava::Get();
  static csJavaCallVoid show (DEF_ARGS, "show");
  return show.call(env);
}

void csJavaWindow::Hide()
{
  JNIEnv *env = csJava::Get();
  static csJavaCallVoid hide (DEF_ARGS, "hide");
  return hide.call(env);

}

void csJavaWindow::Present()
{
  JNIEnv *env = csJava::Get();
  static csJavaCallVoid present (DEF_ARGS, "present");
  return present.call(env);

}

void csJavaWindow::ProcessUpdates()
{
  JNIEnv *env = csJava::Get();
  static csJavaCallVoid processUpdates (DEF_ARGS, "processUpdates");
  return processUpdates.call(env);

}
} // cs