
#include <spcCore/entity/world.hh>
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


}