

#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>
#include <spcCore/graphics/erenderpass.hh>
#include <vector>

namespace spc
{

struct iDevice;
class GfxMesh;
class GfxLight;


SPC_CLASS()
class SPC_CORE_API GfxScene : public SPC_SUPER(iObject)
{
  SPC_CLASS_GEN_OBJECT;
public:
  GfxScene();
  
  void Add(GfxMesh * sceneMesh);
  void Remove(GfxMesh * sceneMesh);
  void Add(GfxLight* light);
  void Remove(GfxLight* light);
  
  void Render(iDevice * device, eRenderPass pass);

private:
  void Add(GfxLight *light, std::vector<GfxLight*> &lights);
  void Remove(GfxLight *light, std::vector<GfxLight*> &lights);


  void AddStaticLightsToMesh(GfxMesh *mesh);
  void AddStaticLightToMeshes(GfxLight *light);
  float CalcInfluenceOfLightToMesh(const GfxLight* light, const GfxMesh* mesh);

  std::vector<GfxMesh*> m_meshes;
  std::vector<GfxMesh*> m_staticMeshes;

  std::vector<GfxLight*> m_globalLights;
  std::vector<GfxLight*> m_staticLights;
  std::vector<GfxLight*> m_dynamicLights;
};

}

