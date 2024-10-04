
#pragma once

#include <csJavaLWJGL/csJavaExport.hh>
#include <csJavaLWJGL/csLwjglKeyboard.hh>
#include <csJavaLWJGL/csLwjglMouse.hh>
#include <csCore/window/iWindow.hh>

#include <jni.h>

namespace cryo::java
{

CS_CLASS(jclass="org.crimsonedge.launcher.LwjglWindow")
class CS_JAVA_API csLwjglWindow : public CS_SUPER(iWindow)
{
  CS_CLASS_GEN_OBJECT;
public:
  csLwjglWindow();
  ~csLwjglWindow() override = default;

  static csLwjglWindow* Get();
  
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
  void SetWindowIcon(const csResourceLocator &iconName) override;

  void Show () override;
  void Hide () override;

  void Present () override;

  void ProcessUpdates() override;


  csLwjglMouse *GetMouse();
  csLwjglKeyboard *GetKeyboard();
private:
  mutable std::string m_title;

  csLwjglKeyboard m_keyboard;
  csLwjglMouse    m_mouse;

  static csLwjglWindow * s_instance;

};

}

