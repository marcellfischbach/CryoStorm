
#include <spcCore/entity/world.hh>
#include <spcCore/graphics/scene/scene.hh>

namespace spc
{

World::World()
  : iObject()
  , m_scene(nullptr)
{
  SPC_CLASS_GEN_CONSTR;
  SetScene(new Scene());
}

void World::SetScene(Scene *scene)
{
  SPC_SET(m_scene, scene);
}

Scene* World::GetScene()
{
  return m_scene;
}

const Scene* World::GetScene() const
{
  return m_scene;
}


}