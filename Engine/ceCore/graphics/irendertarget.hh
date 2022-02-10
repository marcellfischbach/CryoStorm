#pragma  once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/graphics/etexturetype.hh>

namespace ce
{

struct iTexture;

CE_CLASS()
struct CE_CORE_API iRenderTarget : public CE_SUPER(iObject)
{
  CE_CLASS_GEN;
  ~iRenderTarget() override = default;

  virtual eTextureType GetType() const = 0;

  CE_NODISCARD virtual bool Compile() = 0;
  CE_NODISCARD virtual std::string GetCompileLog() const = 0;
};


}