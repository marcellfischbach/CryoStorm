//
// Created by Marcell on 13.03.2025.
//

#include <csCore/loaders/scene/csEntityStateLoaderBase.hh>
#include <algorithm>

namespace cs
{


csEntityStateLoaderRegistry *csEntityStateLoaderRegistry::static_instance = nullptr;

csEntityStateLoaderBase::csEntityStateLoaderBase(int priority)
    : iObject()
    , m_priority(priority)
{

}

void csEntityStateLoaderBase::RegisterClass(const cs::csClass *cls)
{
  m_validClasses.insert(cls);
}

int csEntityStateLoaderBase::Priority()
{
  return m_priority;
}

bool csEntityStateLoaderBase::CanLoad(const cs::csClass *cls) const
{
  return std::find(m_validClasses.begin(), m_validClasses.end(), cls) != m_validClasses.end();
}


csEntityStateLoaderRegistry &csEntityStateLoaderRegistry::Get()
{
  if (!static_instance)
  {
    static_instance = new csEntityStateLoaderRegistry();
  }
  return *static_instance;
}

csEntityStateLoaderRegistry &csEntityStateLoaderRegistry::Register(cs::csEntityStateLoaderBase *loader)
{
  m_stateLoaders.push_back(loader);
  std::sort(m_stateLoaders.begin(),
            m_stateLoaders.end(),
            [](csEntityStateLoaderBase *loader0, csEntityStateLoaderBase *loader1)
            {
              return loader0->Priority() > loader1->Priority();
            }
  );
  return *this;
}


const std::vector<csEntityStateLoaderBase *> &csEntityStateLoaderRegistry::GetStateLoaders() const
{
  return m_stateLoaders;
}


} // cs