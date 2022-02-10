

#include <ceCore/classregistry.hh>


namespace ce
{


ClassRegistry::ClassRegistry()
{

}

ClassRegistry* ClassRegistry::Get()
{
  static ClassRegistry reg;
  return &reg;
}

void ClassRegistry::Register(const Class* clazz)
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

void ClassRegistry::Unregister(const Class* clazz)
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

const std::vector<const Class*>& ClassRegistry::GetAllClasses() const
{
  return m_allClasses;
}


const Class* ClassRegistry::GetClass(const std::string & name) const
{
  std::map<std::string, const Class*>::const_iterator it = m_classes.find(name);
  if (it == m_classes.end())
  {
    return 0;
  }
  return it->second;
}

void* ClassRegistry::CreateClass(const std::string & name) const
{
  const Class* clazz = GetClass(name);
  if (!clazz)
  {
    return 0;
  }

  return clazz->CreateInstance();
}

}

