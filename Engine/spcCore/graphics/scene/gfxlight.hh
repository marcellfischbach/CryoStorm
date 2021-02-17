

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

  void SetFrame(UInt64 frame);
  SPC_NODISCARD UInt64 GetFrame() const;


private:
  bool m_static;
  iLight* m_light;

  UInt64  m_frame;

};



}