
#pragma once

#include <ceCore/csCoreExport.hh>
#include <ceCore/resource/iArchive.hh>

namespace cryo
{

CS_CLASS()
class CS_CORE_API csFileSystemArchive : public CS_SUPER(iArchive)
{
CS_CLASS_GEN_OBJECT;
public:
  csFileSystemArchive(const std::string &name, const std::string &rootPath, int priority);
  ~csFileSystemArchive() override = default;

  const std::string &GetRootPath () const;

  CS_NODISCARD const std::string &GetName () const override;
  int GetPriority() const override;
  iFile *Open(const std::string &locator, eAccessMode accessMode, eOpenMode openMode) override;

private:
  static std::string Resolve (const std::string &rootPath);

  std::string m_name;
  int         m_priority;
  std::string m_rootPath;
};

} // ce
