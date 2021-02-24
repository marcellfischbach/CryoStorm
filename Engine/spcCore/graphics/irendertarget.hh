#pragma  once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>
#include <spcCore/graphics/etexturetype.hh>

namespace spc
{

struct iTexture;

SPC_CLASS()
struct SPC_CORE_API iRenderTarget : public SPC_SUPER(iObject)
{
  SPC_CLASS_GEN;
  ~iRenderTarget() override = default;

  virtual eTextureType GetType() const = 0;

  SPC_NODISCARD virtual bool Compile() = 0;
  SPC_NODISCARD virtual std::string GetCompileLog() const = 0;
};


}