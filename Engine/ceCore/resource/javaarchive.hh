
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/resource/iarchive.hh>

namespace ce
{

CS_CLASS(jclass="org.crimsonedge.core.resource.JavaArchive")
class JavaArchive : public CS_SUPER(iArchive)
{
  CS_CLASS_GEN_OBJECT;
public:
  JavaArchive()  = default;
  JavaArchive(const std::string &name, const std::string &rootPath, int priority);
  ~JavaArchive() override = default;

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