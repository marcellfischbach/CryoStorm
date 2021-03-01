
#include <spcOpenGL/gl4/gl4directionallight.hh>


namespace spc::opengl
{

GL4DirectionalLight::GL4DirectionalLight()
  : iDirectionalLight()
  , m_changeMode(eLCM_Dynamic)
  , m_castShadow(true)
  , m_color(1.0f, 1.0f, 1.0f)
  , m_direction(0.0f, 0.0f, 1.0f)
{

}

GL4DirectionalLight::~GL4DirectionalLight()
{

}


void GL4DirectionalLight::SetChangeMode(eLightChangeMode changeMode)
{
  m_changeMode = changeMode;
}

eLightChangeMode GL4DirectionalLight::GetChangeMode() const
{
  return m_changeMode;
}

void GL4DirectionalLight::SetCastShadow(bool castShadow)
{
  m_castShadow = castShadow;
}

bool GL4DirectionalLight::IsCastShadow() const
{
  return m_castShadow;
}

void GL4DirectionalLight::SetColor(const Color4f& color)
{
  m_color = color;
  m_intensity = color.r + color.g + color.b;
}

const Color4f& GL4DirectionalLight::GetColor() const
{
  return m_color;
}

float GL4DirectionalLight::GetIntensity() const
{
  return m_intensity;
}

void GL4DirectionalLight::SetDirection(const Vector3f& direction)
{
  m_direction = direction;
}

const Vector3f& GL4DirectionalLight::GetDirection() const
{
  return m_direction;
}

}
