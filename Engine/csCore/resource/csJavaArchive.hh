
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/resource/iArchive.hh>

namespace cs
{

CS_CLASS(jclass = "org.cryo.core.resource.JavaArchive")
class csJavaArchive : public CS_SUPER(iArchive)
{
CS_CLASS_GEN_OBJECT;
public:
  csJavaArchive() = default;
  csJavaArchive(const std::string &name, const std::string &rootPath, int priority);
  ~csJavaArchive() override = default;

  const std::string &GetName() const override;
  void SetRootPath(const std::string &rootPath);
  void SetPriority(int priority);
  int GetPriority() const override;
  csOwned<iFile> Open(const std::string &locator, eAccessMode accessMode, eOpenMode openMode) override;

  bool IsExisting(const std::string &locator) const override;
private:
  std::string m_name;
  std::string m_rootPath;
  int         m_priority;

  struct csJavaArchivePrivate *m_priv;
};

}