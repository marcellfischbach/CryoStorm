#ifndef OBJECTREGISTRY_HH
#define OBJECTREGISTRY_HH

#include <SpiceCore/coreexport.hh>
#include <map>
#include <SpiceRefl/class.hh>

namespace Spice
{


class SPICE_CORE_API ObjectRegistry
{
public:
    static ObjectRegistry* Get();

    void Register (const Class* cls, iObject *obj);
    iObject *Get(const Class* cls) const;

    template<typename T> T* Get() const
    {
        iObject *obj = Get(T::GetStaticClass());
        return obj ? static_cast<T*>(obj->QueryClass(T::GetStaticClass())) : nullptr;
    }

private:
    ObjectRegistry();

private:
    std::map<const Class*, iObject*> m_registry;
};

}

#endif // OBJECTREGISTRY_HH
