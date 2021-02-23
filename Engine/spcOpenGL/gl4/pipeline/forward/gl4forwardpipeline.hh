
#pragma once

#include <spcOpenGL/openglexport.hh>
#include <spcCore/graphics/irenderpipeline.hh>
#include <spcCore/graphics/scene/gfxmesh.hh>
#include <spcCore/types.hh>
#include <vector>

namespace spc
{

class GfxLight;
class GfxMesh;


namespace opengl
{

SPC_CLASS()
class SPC_OGL_API GL4ForwardPipeline : public SPC_SUPER(iRenderPipeline)
{
SPC_CLASS_GEN_OBJECT;
public:
  GL4ForwardPipeline();
  ~GL4ForwardPipeline() noexcept override;


  void Render(Camera& camera, Projector& projector, iDevice* device, GfxScene* scene) override;

private:
  void LightScanned(GfxLight * light);
  void MeshScanned(GfxMesh * mesh, const GfxLight * *lights, Size offset);

  Size AssignLights(GfxMesh * mesh,
    const std::vector<GfxMesh::Light> & staticLights,
    const std::vector<GfxMesh::Light> & dynamicLights,
    const GfxLight * *lights,
    Size offset);


  float CalcMeshLightInfluence(const GfxLight * light, const GfxMesh * mesh) const;
  std::vector<GfxMesh::Light> CalcMeshLightInfluences(const GfxMesh * mesh, const std::vector<GfxLight*> & lights, bool sorted) const;
  void AppendLights(GfxMesh * mesh, const std::vector<GfxLight*> & lights) const;


  UInt64 m_frame;
  iDevice* m_device;

  std::vector<GfxLight*> m_dynamicLights;
  std::vector<GfxLight*> m_staticLights;
  std::vector<GfxLight*> m_staticLightsNew;

};

}

}
