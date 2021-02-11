

#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>
#include <spcCore/math/vector3f.hh>

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
  bool IsStatic() const;

  void SetLight(iLight * light);
  iLight* GetLight() ;
  const iLight* GetLight() const;


private:
  bool m_static;
  iLight* m_light;
};



}