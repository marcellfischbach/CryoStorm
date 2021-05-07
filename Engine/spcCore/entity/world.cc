
#include <spcCore/entity/world.hh>
#include <spcCore/entity/entity.hh>
#include <spcCore/entity/spatialstate.hh>
#include <spcCore/graphics/scene/gfxscene.hh>

namespace spc
{

World::World()
  : iObject()
  , m_scene(nullptr)
  , m_rootState(new SpatialState())
{
  SPC_CLASS_GEN_CONSTR;
  SetScene(new GfxScene());
}

void World::SetScene(GfxScene *scene)
{
  SPC_SET(m_scene, scene);
}

GfxScene* World::GetScene()
{
  return m_scene;
}

const GfxScene* World::GetScene() const
{
  return m_scene;
}


bool World::Attach(Entity* entity)
{
  if (std::find(m_entities.begin(), m_entities.end(), entity) != m_entities.end()
    || entity->GetWorld())
  {
    return false;
  }
  m_entities.push_back(entity);
  entity->SetWorld(this);
  entity->AddRef();
  return true;
}

bool World::Detach(Entity* entity)
{
  auto it = std::find(m_entities.begin(), m_entities.end(), entity);
  if (it == m_entities.end() || entity->GetWorld() != this)
  {
    return false;
  }

  m_entities.erase(it);
  entity->SetWorld(nullptr);
  entity->Release();
  return true;
}

bool World::AttachUpdateState(EntityState *updateState)
{
  if (!updateState)
  {
    return false;
  }

  auto it = std::find(m_updateStates.begin(), m_updateStates.end(), updateState);
  if (it != m_updateStates.end())
  {
    return false;
  }

  m_updateStates.push_back(updateState);
  updateState->AddRef();
  return true;
}


bool World::DetachUpdateState(EntityState *updateState)
{
  if (!updateState)
  {
    return false;
  }
  auto it = std::find(m_updateStates.begin(), m_updateStates.end(), updateState);
  if (it == m_updateStates.end())
  {
    return false;
  }

  m_updateStates.erase(it);
  updateState->Release();
  return true;
}


void World::Update(float tpf)
{
  for (auto updateState : m_updateStates)
  {
    updateState->Update(tpf);
  }
}

}