#pragma once


#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <map>

namespace cs
{

class CS_CORE_API csClassRegistry
{
public:
  static csClassRegistry* Get();

  void Register(const csClass* clazz);
  void Unregister(const csClass* clazz);

  const std::vector<const csClass*>& GetAllClasses() const;

  const csClass* GetClass(const std::string& className) const;
  void* CreateClass(const std::string& className) const;

  template<typename T>
  T* CreateClass(const std::string& className) const
  {
    return reinterpret_cast<T*>(CreateClass(className));
  }

private:
  csClassRegistry();

  std::vector<const csClass*>           m_allClasses;
  std::map<std::string, const csClass*> m_classes;
};


}
