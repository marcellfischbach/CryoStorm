#include <spcCore/objectregistry.hh>


namespace spc
{

ObjectRegistry::ObjectRegistry()
{

}

void ObjectRegistry::Register(const Class *cls, iObject *obj)
{
  std::map<const Class *, iObject *> &registry = ObjectRegistry::Get()->m_registry;

  auto it = registry.find(cls);
  if (it != registry.end())
  {
    SPC_RELEASE(it->second);
    registry.erase(it);
  }

  SPC_ADDREF(obj);
  registry[cls] = obj;
}

iObject *ObjectRegistry::Get(const Class *cls)
{
  ObjectRegistry *reg = ObjectRegistry::Get();
  auto it = reg->m_registry.find(cls);
  if (it == reg->m_registry.end())
  {
    return nullptr;
  }
  return it->second;
}

ObjectRegistry *ObjectRegistry::Get()
{
  static ObjectRegistry reg;
  return &reg;
}

}
