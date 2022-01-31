//
// Created by MCEL on 31.01.2022.
//

#include <spcCore/graphics/scene/gfxquadtreescene.hh>

namespace spc
{

GfxQuadtreeScene::GfxQuadtreeScene()
{

}

void GfxQuadtreeScene::Add(GfxCamera *camera)
{

}

void GfxQuadtreeScene::Remove(GfxCamera *camera)
{

}

void GfxQuadtreeScene::Add(GfxMesh *sceneMesh)
{

}

void GfxQuadtreeScene::Remove(GfxMesh *sceneMesh)
{

}

void GfxQuadtreeScene::Add(GfxLight *light)
{

}

void GfxQuadtreeScene::Remove(GfxLight *light)
{

}

const std::vector<GfxCamera *> &GfxQuadtreeScene::GetCameras() const
{
  return m_cameras;
}

void GfxQuadtreeScene::ScanMeshes(const iClipper *clipper,
                                  uint32_t scanMask,
                                  const std::function<void(GfxMesh *)> &callback
                                 ) const
{

}

void GfxQuadtreeScene::ScanLights(const iClipper *clipper,
                                  uint32_t scanMask,
                                  const std::function<bool(GfxLight *)> &callback
                                 ) const
{

}


}
