#include <SpiceCore/objectregistry.hh>


namespace Spice
{

ObjectRegistry::ObjectRegistry()
{

}

void ObjectRegistry::Register(const Class *cls, iObject *obj)
{
    SPICE_ADDREF(obj);
    printf ("Reg: %p -> %p\n", cls, obj);
    m_registry[cls] = obj;
}

iObject *ObjectRegistry::Get(const Class* cls) const
{
    auto it = m_registry.find(cls);
    if (it == m_registry.end())
    {
      printf ("Get: %p -> null\n", cls);
        return nullptr;
    }
    printf ("Get: %p -> %p\n", cls, it->second);
    return it->second;
}

ObjectRegistry* ObjectRegistry::Get()
{
    static ObjectRegistry reg;
    printf ("ObjectRegistry::Get: %p\n", &reg);
    return &reg;
}

}
