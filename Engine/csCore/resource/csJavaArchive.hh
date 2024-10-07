
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/resource/iArchive.hh>

namespace cs
{

CS_CLASS(jclass="org.crimsonedge.core.resource.JavaArchive")
class csJavaArchive : public CS_SUPER(iArchive)
{
  CS_CLASS_GEN_OBJECT;
public:
  csJavaArchive()  = default;
  csJavaArchive(const std::string &name, const std::string &rootPath, int priority);
  ~csJavaArchive() override = default;

  virtual const std::string& GetName() const override;
  void SetRootPath(const std::string &rootPath);
  void SetPriority(int priority);
  int GetPriority() const override;
  iFile *Open(const std::string &locator, eAccessMode accessMode, eOpenMode openMode) override;

private:
  std::string m_name;
  std::string m_rootPath;
  int m_priority;
};

}