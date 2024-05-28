#pragma  once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>


namespace ce
{

struct iDevice;
struct iTextureCube;

CE_CLASS()
struct CE_CORE_API iSkyboxRenderer : CE_SUPER(iObject)
{
  CE_CLASS_GEN;

  ~iSkyboxRenderer() override = default;

  virtual void Render(iDevice* device) = 0;

  virtual iTextureCube *GetTexture () const = 0;

};


}