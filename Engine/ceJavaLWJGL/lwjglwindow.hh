
#pragma once

#include <ceJavaLWJGL/javaexport.hh>
#include <ceJavaLWJGL/lwjglkeyboard.hh>
#include <ceJavaLWJGL/lwjglmouse.hh>
#include <ceCore/window/iwindow.hh>

#include <jni.h>

namespace ce::java
{

CE_CLASS(jclass="org.crimsonedge.launcher.LwjglWindow")
class CE_JAVA_API LwjglWindow : public CE_SUPER(iWindow)
{
  CE_CLASS_GEN_OBJECT;
public:
  LwjglWindow();
  ~LwjglWindow() override = default;

  static LwjglWindow* Get();
  
  void SetTitle(const std::string& title) override;
  const std::string &GetTitle() const override;

  void SetPosition (int16_t x, int16_t y) override;
  int GetPositionX() const override;
  int GetPositionY() const override;

  void SetResolution(uint16_t width, uint16_t height) override;
  int GetWidth() const override;
  int GetHeight() const override;


  /**
   * Optional.
   * @param iconName
   */
  void SetWindowIcon (const ResourceLocator &iconName) override;

  void Show () override;
  void Hide () override;

  void Present () override;

  void ProcessUpdates() override;


  LwjglMouse *GetMouse ();
  LwjglKeyboard *GetKeyboard ();
private:
  mutable std::string m_title;

  LwjglKeyboard m_keyboard;
  LwjglMouse m_mouse;

  static LwjglWindow* s_instance;

};

}

