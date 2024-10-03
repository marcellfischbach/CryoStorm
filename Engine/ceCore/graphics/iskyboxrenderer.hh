#pragma  once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>


namespace ce
{

struct iDevice;
struct iTextureCube;

CS_CLASS()
struct CS_CORE_API iSkyboxRenderer : CS_SUPER(iObject)
{
  CS_CLASS_GEN;

  ~iSkyboxRenderer() override = default;

  virtual void Render(iDevice* device) = 0;

  virtual iTextureCube *GetTexture () const = 0;

};


}