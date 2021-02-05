
#include <spcCore/entity/world.hh>
#include <spcCore/entity/entity.hh>
#include <spcCore/entity/spatialstate.hh>
#include <spcCore/graphics/scene/gfxscene.hh>

namespace spc
{

World::World()
  : iObject()
  , m_scene(nullptr)
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

void World::UpdateTransformation()
{
  for (auto entity : m_entities)
  {
    SpatialState *spatialState = entity->GetRoot();
    if (spatialState)
    {
      spatialState->UpdateTransformation();
    }
  }
}

void World::Update(float tpf)
{
  for (auto entity : m_entities)
  {
    entity->Update(tpf);
  }
}

}