#include <ceCore/csObjectRegistry.hh>


namespace cryo
{

csDebugCache::csDebugCache()
{
  CS_CLASS_GEN_CONSTR;
}

csObjectRegistry::csObjectRegistry()
{

}

void csObjectRegistry::Register(const csClass *cls, iObject *obj)
{
  std::map<const csClass *, iObject *> &registry = csObjectRegistry::Get()->m_registry;

  auto it = registry.find(cls);
  if (it != registry.end())
  {
    CS_RELEASE(it->second);
    registry.erase(it);
  }

  CS_ADDREF(obj);
  registry[cls] = obj;
}

iObject *csObjectRegistry::Get(const csClass *cls)
{
  csObjectRegistry *reg = csObjectRegistry::Get();
  auto             it   = reg->m_registry.find(cls);
  if (it == reg->m_registry.end())
  {
    return nullptr;
  }
  return it->second;
}

csObjectRegistry *csObjectRegistry::Get()
{
  static csObjectRegistry reg;
  return &reg;
}

}
