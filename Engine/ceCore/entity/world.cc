
#include <ceCore/entity/world.hh>
#include <ceCore/entity/camerastate.hh>
#include <ceCore/entity/entity.hh>
#include <ceCore/entity/spatialstate.hh>
#include <ceCore/graphics/scene/igfxscene.hh>
#include <ceCore/graphics/scene/gfxsimplescene.hh>
#include <ceCore/graphics/scene/gfxquadtreescene.hh>
#include <ceCore/physics/physics.hh>
#include <ceCore/objectregistry.hh>

#include <atomic>
#include <thread>


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

iGfxScene *World::GetScene()
{
  return m_scene;
}

const iGfxScene *World::GetScene() const
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

void World::SetMainCamera(CameraState *mainCamera)
{
  CE_SET(m_mainCamera, mainCamera);
}

CameraState *World::GetMainCamera()
{
  return m_mainCamera;
}

const CameraState *World::GetMainCamera() const
{
  return m_mainCamera;
}

bool World::Attach(Entity *entity)
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

bool World::Detach(Entity *entity)
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



struct ThreadData
{
  ::std::vector<EntityState *> *updateStates;
  ::std::atomic_size_t counter;
  ::std::atomic_bool inUpdate;
  ::std::atomic_bool active;
  float tpf;
};


static ThreadData s_threadData;

void ThreadLoop()
{
  while (s_threadData.active.load())
  {
    while (s_threadData.inUpdate.load())
    {
      size_t idx = s_threadData.counter.fetch_add(1);
      if (idx >= s_threadData.updateStates->size())
      {
        s_threadData.inUpdate.store(false);
        break;
      }
      EntityState *pState = (*s_threadData.updateStates)[idx];
      pState->Update(s_threadData.tpf);
    }
  }
}

static bool s_threadsSpawned = false;

void World::Update(float tpf)
{
  if (true)
  {
    if (!s_threadsSpawned)
    {
      s_threadsSpawned = true;
      s_threadData.inUpdate.store(false);
      s_threadData.active.store(true);

      for (unsigned i = 0; i < m_threads.size(); i++)
      {
        m_threads[i] = std::thread(ThreadLoop);
      }

    }

    s_threadData.inUpdate.store(false);
    s_threadData.updateStates = &m_updateStates;
    s_threadData.counter.store(0);
    s_threadData.inUpdate.store(true);
    s_threadData.tpf = tpf;
    while(s_threadData.inUpdate.load());

  }
  else
  {
    for (auto updateState: m_updateStates)
    {
      updateState->Update(tpf);
    }
  }

  m_physicsUpdateCounter += tpf;
  while (m_physicsUpdateCounter >= (1.0f / 60.0f))
  {
    m_physicsUpdateCounter -= (1.0f / 60.0f);
    m_physicsWorld->Step(1.0f / 60.0f);
  }


  const std::vector<ce::iPhysicsWorld::DynamicResult> &result = m_physicsWorld->SwapResult();
  for (auto &res: result)
  {
    res.Collider->GetUserData()->SetLocalMatrix(res.Matrix);
  }
}

}