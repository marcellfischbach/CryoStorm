
#include <spcOpenGL/gl4/gl4pointlight.hh>


namespace spc
{

GL4PointLight::GL4PointLight()
  : iPointLight()
  , m_changeMode(eLCM_Dynamic)
  , m_castShadow(true)
  , m_color(1.0f, 1.0f, 1.0f)
  , m_position(0.0f, 0.0f, 0.0f)
  , m_range(10.0)
{

}

GL4PointLight::~GL4PointLight()
{

}


void GL4PointLight::SetChangeMode(eLightChangeMode changeMode)
{
  m_changeMode = changeMode;
}

eLightChangeMode GL4PointLight::GetChangeMode() const
{
  return m_changeMode;
}

void GL4PointLight::SetCastShadow(bool castShadow)
{
  m_castShadow = castShadow;
}

bool GL4PointLight::IsCastShadow() const
{
  return m_castShadow;
}

void GL4PointLight::SetColor(const Color4f& color)
{
  m_color = color;
}

const Color4f& GL4PointLight::GetColor() const
{
  return m_color;
}

void GL4PointLight::SetPosition(const Vector3f& position)
{
  m_position = position;
}

const Vector3f& GL4PointLight::GetPosition() const
{
  return m_position;
}


void GL4PointLight::SetRange(float range)
{
  m_range = range;
}


float GL4PointLight::GetRange() const
{
  return m_range;
}

}
