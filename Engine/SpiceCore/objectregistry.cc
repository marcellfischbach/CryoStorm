#include <SpiceCore/objectregistry.hh>


namespace Spice
{

ObjectRegistry::ObjectRegistry()
{

}

void ObjectRegistry::Register(const Class* cls, iObject* obj)
{
  SPICE_ADDREF(obj);
  ObjectRegistry::Get()->m_registry[cls] = obj;
}

iObject* ObjectRegistry::Get(const Class* cls)
{
  ObjectRegistry* reg = ObjectRegistry::Get();
  auto it = reg->m_registry.find(cls);
  if (it == reg->m_registry.end())
  {
    return nullptr;
  }
  return it->second;
}

ObjectRegistry* ObjectRegistry::Get()
{
  static ObjectRegistry reg;
  return &reg;
}

}
