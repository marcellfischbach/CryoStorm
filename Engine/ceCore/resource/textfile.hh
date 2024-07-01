
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/resource/iassetloader.hh>
#include <string>

namespace ce
{


CE_CLASS()
class CE_CORE_API TextFile : public CE_SUPER(iObject)
{
  CE_CLASS_GEN_OBJECT;
public:
  TextFile () = default;
  ~TextFile() override = default;

  void SetContent (const std::string &content);
  const std::string &GetContent () const;

private:

  std::string m_content = "";

};

} // ce
