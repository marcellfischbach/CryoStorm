
#include <ceCore/graphics/scene/gfxoctreescene.hh>


namespace cryo
{

GfxOctreeScene::GfxOctreeScene()
{

}

void GfxOctreeScene::Add(GfxCamera *camera)
{

}

void GfxOctreeScene::Remove(GfxCamera *camera)
{

}

void GfxOctreeScene::Add(GfxMesh *sceneMesh)
{

}

void GfxOctreeScene::Remove(GfxMesh *sceneMesh)
{

}

void GfxOctreeScene::Add(GfxLight *light)
{

}

void GfxOctreeScene::Remove(GfxLight *light)
{

}

const std::vector<GfxCamera *> &GfxOctreeScene::GetCameras() const
{
  return m_cameras;
}

void GfxOctreeScene::ScanMeshes(const iClipper *clipper,
                                uint32_t scanMask,
                                const std::function<void(GfxMesh *)> &callback
                               ) const
{

}

void GfxOctreeScene::ScanLights(const iClipper *clipper,
                                uint32_t scanMask,
                                const std::function<bool(GfxLight *)> &callback
                               ) const
{

}

void GfxOctreeScene::ScanMeshes(const iClipper *clipper, GfxSceneCollector &collector) const
{

}

void GfxOctreeScene::ScanMeshes(const iClipper *clipper, uint32_t scanMask, GfxSceneCollector &collector) const
{

}

}