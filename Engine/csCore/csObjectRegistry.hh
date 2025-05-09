#ifndef OBJECTREGISTRY_HH
#define OBJECTREGISTRY_HH

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/csRef.hh>
#include <map>

namespace cs
{

CS_CLASS()
class CS_CORE_API csDebugCache : public CS_SUPER(iObject)
{
CS_CLASS_GEN_OBJECT;
public:
  csDebugCache() = default;
  ~csDebugCache() override = default;

  void SetDebug(bool debug)
  {
    m_debug = debug;
  }

  bool IsDebug()
  {
    return m_debug;
  }


private:
  bool m_debug;

};


class CS_CORE_API csObjectRegistry
{
public:

  template<typename T>
  static void Register(iObject *obj)
  {
    Register(T::GetStaticClass(), obj);
  }
  static void Register(const csClass *cls, iObject *obj);

  template<typename T>
  static T *Get()
  {
    iObject *obj = Get(T::GetStaticClass());
    return obj ? static_cast<T *>(obj->QueryClass(T::GetStaticClass())) : nullptr;
  }
  static iObject *Get(const csClass *cls);

  template<typename T>
  static void Remove ()
  {
    Remove(T::GetStaticClass());
  }
  static void Remove (const csClass* cls);

private:
  csObjectRegistry() = default;

private:
  static std::map<const csClass *, csRef<iObject>> m_registry;
};

}

#endif // OBJECTREGISTRY_HH
