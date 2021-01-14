
#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>

namespace spc
{

class GfxScene;

SPC_CLASS()
class SPC_CORE_API World : public SPC_SUPER(iObject)
{
  SPC_CLASS_GEN_OBJECT;
public:
  World();
  
  void SetScene(GfxScene * scene);
  GfxScene* GetScene();
  const GfxScene* GetScene() const;
  
private:
  GfxScene* m_scene;
  
  
};

}