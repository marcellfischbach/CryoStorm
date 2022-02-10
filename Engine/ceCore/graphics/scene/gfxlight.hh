

#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/math/vector3f.hh>
#include <ceCore/types.hh>

namespace ce
{

struct iLight;


CE_CLASS()
class CE_CORE_API GfxLight : public CE_SUPER(iObject)
{
  CE_CLASS_GEN_OBJECT;
public:
  GfxLight();
  ~GfxLight() override;

  void SetStatic(bool _static);
  CE_NODISCARD bool IsStatic() const;

  void SetLight(iLight * light);
  iLight* GetLight() ;
  CE_NODISCARD const iLight* GetLight() const;

  void SetFrame(uint64_t frame);
  CE_NODISCARD uint64_t GetFrame() const;


private:
  bool m_static;
  iLight* m_light;

  uint64_t  m_frame;

};



}