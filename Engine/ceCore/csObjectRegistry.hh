#ifndef OBJECTREGISTRY_HH
#define OBJECTREGISTRY_HH

#include <ceCore/csCoreExport.hh>
#include <map>
#include <ceCore/csClass.hh>

namespace cryo
{

CS_CLASS()
class CS_CORE_API csDebugCache : public CS_SUPER(iObject)
{
  CS_CLASS_GEN_OBJECT;
public:
  csDebugCache();
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

    template<typename T> static void Register(iObject* obj)
    {
      Register(T::GetStaticClass(), obj);
    }
    static void Register(const csClass* cls, iObject *obj);

    template<typename T> static T* Get() 
    {
        iObject *obj = Get(T::GetStaticClass());
        return obj ? static_cast<T*>(obj->QueryClass(T::GetStaticClass())) : nullptr;
    }
    static iObject* Get(const csClass* cls);

private:
    csObjectRegistry();

private:
  static csObjectRegistry* Get();
  std::map<const csClass*, iObject*> m_registry;
};

}

#endif // OBJECTREGISTRY_HH
