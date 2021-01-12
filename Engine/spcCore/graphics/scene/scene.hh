

#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>
#include <spcCore/graphics/erenderpass.hh>
#include <vector>

namespace spc
{

struct iDevice;
struct iLight;
class SceneMesh;


SPC_CLASS()
class SPC_CORE_API Scene : public SPC_SUPER(iObject)
{
  SPC_CLASS_GEN_OBJECT;
public:
  Scene();
  
  void Add(SceneMesh* sceneMesh);
  void Add(iLight* light);
  
  void Render(iDevice * device, eRenderPass pass);

private:
  
  std::vector<SceneMesh*> m_meshes;
  std::vector<iLight*> m_lights;
};

}
