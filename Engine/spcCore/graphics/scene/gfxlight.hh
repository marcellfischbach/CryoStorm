

#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>
#include <spcCore/math/vector3f.hh>
#include <spcCore/types.hh>

namespace spc
{

struct iLight;


SPC_CLASS()
class SPC_CORE_API GfxLight : public SPC_SUPER(iObject)
{
  SPC_CLASS_GEN_OBJECT;
public:
  GfxLight();
  ~GfxLight() override;

  void SetStatic(bool _static);
  SPC_NODISCARD bool IsStatic() const;

  void SetLight(iLight * light);
  iLight* GetLight() ;
  SPC_NODISCARD const iLight* GetLight() const;

  void SetFrame(uint64_t frame);
  SPC_NODISCARD uint64_t GetFrame() const;


private:
  bool m_static;
  iLight* m_light;

  uint64_t  m_frame;

};



}