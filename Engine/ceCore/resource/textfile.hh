
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/resource/iassetloader.hh>
#include <string>

namespace cryo
{


CS_CLASS()
class CS_CORE_API TextFile : public CS_SUPER(iObject)
{
  CS_CLASS_GEN_OBJECT;
public:
  TextFile () = default;
  ~TextFile() override = default;

  void SetContent (const std::string &content);
  const std::string &GetContent () const;

private:

  std::string m_content = "";

};

} // ce
