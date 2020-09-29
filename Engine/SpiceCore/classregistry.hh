#pragma once


#include <SpiceCore/coreexport.hh>
#include <SpiceCore/class.hh>
#include <map>

namespace Spice
{

class SPICE_CORE_API ClassRegistry
{
public:
  static ClassRegistry* Get();

  void Register(const Class* clazz);
  void Unregister(const Class* clazz);

  const std::vector<const Class*>& GetAllClasses() const;

  const Class* GetClass(const std::string& className) const;
  void* CreateClass(const std::string& className) const;

  template<typename T>
  T* CreateClass(const std::string& className) const
  {
    return reinterpret_cast<T*>(CreateClass(className));
  }

private:
  ClassRegistry();

  std::vector<const Class*> m_allClasses;
  std::map<std::string, const Class*> m_classes;
};


}
