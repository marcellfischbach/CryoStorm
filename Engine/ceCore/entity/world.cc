
#include <ceCore/entity/world.hh>
#include <ceCore/entity/entity.hh>
#include <ceCore/entity/spatialstate.hh>
#include <ceCore/graphics/scene/igfxscene.hh>
#include <ceCore/graphics/scene/gfxsimplescene.hh>
#include <ceCore/graphics/scene/gfxquadtreescene.hh>
#include <ceCore/physics/physics.hh>
#include <ceCore/objectregistry.hh>

namespace ce
{

World::World()
  : iObject()
  , m_scene(nullptr)
  , m_physicsWorld(nullptr)
  , m_physicsUpdateCounter(0.0f)
  , m_rootState(new SpatialState())
{
  CE_CLASS_GEN_CONSTR;
  SetScene(new GfxQuadtreeScene());
  SetPhysicsWorld(ObjectRegistry::Get<iPhysicsSystem>()->CreateWorld());
}

void World::SetScene(iGfxScene *scene)
{
  CE_SET(m_scene, scene);
}

iGfxScene* World::GetScene()
{
  return m_scene;
}

const iGfxScene* World::GetScene() const
{
  return m_scene;
}

void World::SetPhysicsWorld(iPhysicsWorld *world)
{
  CE_SET(m_physicsWorld, world);
}

iPhysicsWorld *World::GetPhysicsWorld()
{
  return m_physicsWorld;
}


const iPhysicsWorld *World::GetPhysicsWorld() const
{
  return m_physicsWorld;
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

  m_physicsUpdateCounter += tpf;
  while (m_physicsUpdateCounter >= (1.0f / 60.0f))
  {
    m_physicsUpdateCounter-= (1.0f / 60.0f);
    m_physicsWorld->Step(1.0f / 60.0f);
  }
}

}