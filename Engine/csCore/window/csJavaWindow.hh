//
// Created by Marcell on 22.10.2024.
//

#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/window/iWindow.hh>

namespace cs
{


CS_CLASS(jclass="org.cryo.core.window.JavaWindow")
class CS_CORE_API csJavaWindow : public CS_SUPER(cs::iWindow)
{
  CS_CLASS_GEN_OBJECT;
public:
  csJavaWindow () = default;
  ~csJavaWindow() = default;

  void SetTitle(const std::string& title) override;
  const std::string &GetTitle() const override;

  void SetPosition (int16_t x, int16_t y) override;
  int16_t GetPositionX() const override;
  int16_t GetPositionY() const override;

  void SetResolution(uint16_t width, uint16_t height) override;
  int16_t GetWidth() const override;
  int16_t GetHeight() const override;


  /**
   * Optional.
   * @param iconName
   */
  void SetWindowIcon(const csResourceLocator &iconName) override;

  void Show () override;
  void Hide () override;

  void Present () override;

  void ProcessUpdates() override;

private:
  mutable std::string m_title;

};

} // cs
