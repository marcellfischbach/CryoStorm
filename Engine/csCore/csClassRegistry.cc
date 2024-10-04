

#include <csCore/csClassRegistry.hh>


namespace cryo
{


csClassRegistry::csClassRegistry()
{

}

csClassRegistry* csClassRegistry::Get()
{
  static csClassRegistry reg;
  return &reg;
}

void csClassRegistry::Register(const csClass* clazz)
{
  if (!clazz)
  {
    return;
  }

  if (m_classes.find(clazz->GetName()) != m_classes.end())
  {
    return;
  }
  m_allClasses.push_back(clazz);
  m_classes[clazz->GetName()] = clazz;
}

void csClassRegistry::Unregister(const csClass* clazz)
{
  if (!clazz)
  {
    return;
  }

  auto it = m_classes.find(clazz->GetName());
  if (it != m_classes.end())
  {
    m_classes.erase(it);
  }
  for (size_t i = 0, in = m_allClasses.size(); i < in; ++i)
  {
    if (m_allClasses[i] == clazz)
    {
      m_allClasses.erase(m_allClasses.begin() + i);
      break;
    }
  }
}

const std::vector<const csClass*>& csClassRegistry::GetAllClasses() const
{
  return m_allClasses;
}


const csClass* csClassRegistry::GetClass(const std::string & name) const
{
  std::map<std::string, const csClass*>::const_iterator it = m_classes.find(name);
  if (it == m_classes.end())
  {
    return 0;
  }
  return it->second;
}

void* csClassRegistry::CreateClass(const std::string & name) const
{
  const csClass * clazz = GetClass(name);
  if (!clazz)
  {
    return 0;
  }

  return clazz->CreateInstance();
}

}

