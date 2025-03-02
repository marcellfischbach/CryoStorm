
#include <csCore/entity/csWorld.hh>
#include <csCore/entity/csWorld.refl.cc>
#include <csCore/entity/csCameraState.hh>
#include <csCore/entity/csEntity.hh>
#include <csCore/entity/csSpatialState.hh>
#include <csCore/graphics/scene/iGfxScene.hh>
#include <csCore/graphics/scene/csGfxSimpleScene.hh>
#include <csCore/graphics/scene/csGfxQuadtreeScene.hh>
#include <csCore/physics/csPhysics.hh>
#include <csCore/csObjectRegistry.hh>

#include <atomic>
#include <thread>


namespace cs
{


csWorld::csWorld()
    : iObject()
    , m_scene(nullptr)
    , m_physicsWorld(nullptr)
    , m_physicsUpdateCounter(0.0f)
    , m_rootState(new csSpatialState())
{

  SetScene(new csGfxQuadtreeScene());
  SetPhysicsWorld(csObjectRegistry::Get<iPhysicsSystem>()->CreateWorld().Data());
}


void csWorld::SetScene(iGfxScene *scene)
{
  m_scene = scene;
}

iGfxScene *csWorld::GetScene()
{
  return m_scene;
}

const iGfxScene *csWorld::GetScene() const
{
  return m_scene;
}

void csWorld::SetPhysicsWorld(iPhysicsWorld *world)
{
  m_physicsWorld = world;
}

iPhysicsWorld *csWorld::GetPhysicsWorld()
{
  return m_physicsWorld;
}


const iPhysicsWorld *csWorld::GetPhysicsWorld() const
{
  return m_physicsWorld;
}

void csWorld::SetMainCamera(csCameraState *mainCamera)
{
  m_mainCamera = mainCamera;
}

csCameraState *csWorld::GetMainCamera()
{
  return m_mainCamera;
}

const csCameraState *csWorld::GetMainCamera() const
{
  return m_mainCamera;
}

bool csWorld::Attach(csEntity *entity)
{
  if (std::find(m_entities.begin(), m_entities.end(), entity) != m_entities.end()
      || entity->GetWorld())
  {
    return false;
  }

  m_entities.push_back(entity);
  entity->SetWorld(this);
  return true;
}

bool csWorld::Detach(csEntity *entity)
{
  auto it = std::find(m_entities.begin(), m_entities.end(), entity);
  if (it == m_entities.end() || entity->GetWorld() != this)
  {
    return false;
  }

  entity->SetWorld(nullptr);
  m_entities.erase(it);
  return true;
}

bool csWorld::AttachUpdateState(csEntityState *updateState)
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
  return true;
}


bool csWorld::DetachUpdateState(csEntityState *updateState)
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
  return true;
}



struct ThreadData
{
  ::std::vector<csRef<csEntityState>> *updateStates;
  ::std::atomic_size_t           counter;
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
      csEntityState *pState = (*s_threadData.updateStates)[idx];
      pState->Update(s_threadData.tpf);
    }
  }
}

static bool s_threadsSpawned = false;

void csWorld::Update(float tpf)
{
  if (false)
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
  if (m_scene)
  {
    m_scene->Optimize();
  }

  m_physicsUpdateCounter += tpf;
  while (m_physicsUpdateCounter >= (1.0f / 60.0f))
  {
    m_physicsUpdateCounter -= (1.0f / 60.0f);
    m_physicsWorld->Step(1.0f / 60.0f);
  }


  const std::vector<cs::iPhysicsWorld::DynamicResult> &result = m_physicsWorld->SwapResult();
  for (auto &res: result)
  {
    res.Collider->GetUserData()->SetLocalMatrix(res.Matrix);
  }
}

}
