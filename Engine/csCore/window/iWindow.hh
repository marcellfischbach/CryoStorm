#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csTypes.hh>
#include <csCore/csClass.hh>

#include <string>


namespace cryo
{

class csResourceLocator;

CS_CLASS()
struct CS_CORE_API iWindow : public iObject
{
  CS_CLASS_GEN;

  ~iWindow() override {  }

  virtual void SetTitle(const std::string& title) = 0;
  virtual const std::string &GetTitle() const = 0;

  virtual void SetPosition (int16_t x, int16_t y) = 0;
  virtual int GetPositionX() const = 0;
  virtual int GetPositionY() const = 0;

  virtual void SetResolution(uint16_t width, uint16_t height) = 0;
  virtual int GetWidth() const = 0;
  virtual int GetHeight() const = 0;


  /**
   * Optional.
   * @param iconName
   */
  virtual void SetWindowIcon(const csResourceLocator &iconName) = 0;

  virtual void Show () = 0;
  virtual void Hide () = 0;

  virtual void Present () = 0;

  virtual void ProcessUpdates() = 0;

};

}

