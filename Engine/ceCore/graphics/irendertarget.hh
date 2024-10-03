#pragma  once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/graphics/etexturetype.hh>

namespace cryo
{

struct iTexture;

CS_CLASS()
struct CS_CORE_API iRenderTarget : public CS_SUPER(iObject)
{
  CS_CLASS_GEN;
  ~iRenderTarget() override = default;

  virtual eTextureType GetType() const = 0;

  CS_NODISCARD virtual bool Compile() = 0;
  CS_NODISCARD virtual std::string GetCompileLog() const = 0;
};


}