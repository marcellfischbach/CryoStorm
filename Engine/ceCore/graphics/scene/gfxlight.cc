

#include <ceCore/graphics/scene/gfxlight.hh>
#include <ceCore/graphics/ilight.hh>


namespace ce
{

GfxLight::GfxLight()
  : iObject()
  , m_static(false)
  , m_light(nullptr)
  , m_frame(0)
{
  CE_CLASS_GEN_CONSTR;
}

GfxLight::~GfxLight()
{
  CE_RELEASE(m_light);
  m_light = nullptr;
}

void GfxLight::SetStatic(bool _static)
{
  m_static = _static;
}

bool GfxLight::IsStatic() const
{
  return m_static;
}

void GfxLight::SetLight(iLight* light)
{
  CE_SET(m_light, light);
}

iLight * GfxLight::GetLight()
{
  return m_light;
}

const iLight * GfxLight::GetLight() const
{
  return m_light;
}


void GfxLight::SetFrame(uint64_t frame)
{
  m_frame = frame;
}

uint64_t GfxLight::GetFrame() const
{
  return m_frame;
}

}