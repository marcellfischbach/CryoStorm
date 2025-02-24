
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <string>

namespace cs
{


CS_CLASS()
class CS_CORE_API csTextFile : public CS_SUPER(iObject)
{
  CS_CLASS_GEN_OBJECT;
public:
  csTextFile() = default;
  ~csTextFile() override = default;

  void SetContent (const std::string &content);
  const std::string &GetContent () const;

private:

  std::string m_content = "";

};

} // ce
