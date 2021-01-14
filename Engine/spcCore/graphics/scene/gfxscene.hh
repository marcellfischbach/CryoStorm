

#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>
#include <spcCore/graphics/erenderpass.hh>
#include <vector>

namespace spc
{

struct iDevice;
struct iLight;
class GfxSceneMesh;


SPC_CLASS()
class SPC_CORE_API GfxScene : public SPC_SUPER(iObject)
{
  SPC_CLASS_GEN_OBJECT;
public:
  GfxScene();
  
  void Add(GfxSceneMesh * sceneMesh);
  void Add(iLight* light);
  
  void Render(iDevice * device, eRenderPass pass);

private:
  
  std::vector<GfxSceneMesh*> m_meshes;
  std::vector<iLight*> m_lights;
};

}
