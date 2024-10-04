

#include <csCore/graphics/scene/csGfxLight.hh>
#include <csCore/graphics/iLight.hh>


namespace cryo
{

csGfxLight::csGfxLight()
  : iObject()
  , m_static(false)
  , m_light(nullptr)
  , m_frame(0)
{
  CS_CLASS_GEN_CONSTR;
}

csGfxLight::~csGfxLight()
{
  CS_RELEASE(m_light);
  m_light = nullptr;
}

void csGfxLight::SetStatic(bool _static)
{
  m_static = _static;
}

bool csGfxLight::IsStatic() const
{
  return m_static;
}

void csGfxLight::SetLight(iLight* light)
{
  CS_SET(m_light, light);
}

iLight * csGfxLight::GetLight()
{
  return m_light;
}

const iLight * csGfxLight::GetLight() const
{
  return m_light;
}


void csGfxLight::SetFrame(uint64_t frame)
{
  m_frame = frame;
}

uint64_t csGfxLight::GetFrame() const
{
  return m_frame;
}

}