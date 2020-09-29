#ifndef OBJECTREGISTRY_HH
#define OBJECTREGISTRY_HH

#include <SpiceCore/coreexport.hh>
#include <map>
#include <SpiceCore/class.hh>

namespace Spice
{


class SPICE_CORE_API ObjectRegistry
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
