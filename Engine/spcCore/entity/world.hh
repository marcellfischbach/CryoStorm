
#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>

namespace spc
{

class Scene;

SPC_CLASS()
class SPC_CORE_API World : public SPC_SUPER(iObject)
{
  SPC_CLASS_GEN_OBJECT;
public:
  World();
  
  void SetScene(Scene* scene);
  Scene* GetScene();
  const Scene* GetScene() const;
  
private:
  Scene* m_scene;
  
  
};

}