

#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/math/vector3f.hh>
#include <ceCore/types.hh>

namespace cryo
{

struct iLight;


CS_CLASS()
class CS_CORE_API GfxLight : public CS_SUPER(iObject)
{
  CS_CLASS_GEN_OBJECT;
public:
  GfxLight();
  ~GfxLight() override;

  void SetStatic(bool _static);
  CS_NODISCARD bool IsStatic() const;

  void SetLight(iLight * light);
  iLight* GetLight() ;
  CS_NODISCARD const iLight* GetLight() const;

  void SetFrame(uint64_t frame);
  CS_NODISCARD uint64_t GetFrame() const;


private:
  bool m_static;
  iLight* m_light;

  uint64_t  m_frame;

};



}