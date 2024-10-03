#ifndef OBJECTREGISTRY_HH
#define OBJECTREGISTRY_HH

#include <ceCore/coreexport.hh>
#include <map>
#include <ceCore/class.hh>

namespace cryo
{

CS_CLASS()
class CS_CORE_API DebugCache : public CS_SUPER(iObject)
{
  CS_CLASS_GEN_OBJECT;
public:
  DebugCache();
  ~DebugCache() override = default;

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


class CS_CORE_API ObjectRegistry
{
public:

    template<typename T> static void Register(iObject* obj)
    {
      Register(T::GetStaticClass(), obj);
    }
    static void Register (const Class* cls, iObject *obj);

    template<typename T> static T* Get() 
    {
        iObject *obj = Get(T::GetStaticClass());
        return obj ? static_cast<T*>(obj->QueryClass(T::GetStaticClass())) : nullptr;
    }
    static iObject* Get(const Class* cls);

private:
    ObjectRegistry();

private:
  static ObjectRegistry* Get();
  std::map<const Class*, iObject*> m_registry;
};

}

#endif // OBJECTREGISTRY_HH
