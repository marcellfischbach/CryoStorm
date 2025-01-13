#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csTypes.hh>
#include <csCore/csClass.hh>

#include <string>


namespace cs
{

class csAssetLocator;

CS_CLASS(jclass="org.cryo.core.window.IWindow")
struct CS_CORE_API iWindow : public iObject
{
  CS_CLASS_GEN;

  ~iWindow() override {  }

  CS_FUNCTION()
  virtual void SetTitle(const std::string& title) = 0;
  CS_FUNCTION()
  virtual const std::string &GetTitle() const = 0;

  CS_FUNCTION()
  virtual void SetPosition (int16_t x, int16_t y) = 0;
  CS_FUNCTION()
  virtual int16_t GetPositionX() const = 0;
  CS_FUNCTION()
  virtual int16_t GetPositionY() const = 0;

  CS_FUNCTION()
  virtual void SetResolution(uint16_t width, uint16_t height) = 0;
  CS_FUNCTION()
  virtual int16_t GetWidth() const = 0;
  CS_FUNCTION()
  virtual int16_t GetHeight() const = 0;


  /**
   * Optional.
   * @param iconName
   */
  virtual void SetWindowIcon(const csAssetLocator &iconName) = 0;

  CS_FUNCTION()
  virtual void Show () = 0;
  CS_FUNCTION()
  virtual void Hide () = 0;

  CS_FUNCTION()
  virtual void Present () = 0;

  CS_FUNCTION()
  virtual void ProcessUpdates() = 0;

};

}

