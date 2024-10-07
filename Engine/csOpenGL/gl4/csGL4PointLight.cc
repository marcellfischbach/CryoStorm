
#include <csOpenGL/gl4/csGL4PointLight.hh>


namespace cs::opengl
{

csGL4PointLight::csGL4PointLight()
  : iPointLight()
  , m_changeMode(eLCM_Dynamic)
  , m_castShadow(true)
  , m_color(1.0f, 1.0f, 1.0f)
  , m_intensity(1.0)
  , m_position(0.0f, 0.0f, 0.0f)
  , m_range(10.0)
{

}

csGL4PointLight::~csGL4PointLight()
{

}


void csGL4PointLight::SetChangeMode(eLightChangeMode changeMode)
{
  m_changeMode = changeMode;
}

eLightChangeMode csGL4PointLight::GetChangeMode() const
{
  return m_changeMode;
}

void csGL4PointLight::SetCastShadow(bool castShadow)
{
  m_castShadow = castShadow;
}

bool csGL4PointLight::IsCastShadow() const
{
  return m_castShadow;
}

void csGL4PointLight::SetShadowMapBias(float shadowBias)
{
  m_shadowBias = shadowBias;
}

float csGL4PointLight::GetShadowMapBias() const
{
  return m_shadowBias;
}


void csGL4PointLight::SetColor(const csColor4f& color)
{
  m_color = color;
}

const csColor4f& csGL4PointLight::GetColor() const
{
  return m_color;
}

void csGL4PointLight::SetIntensity(float intensity)
{
  m_intensity = intensity;
}

float csGL4PointLight::GetIntensity() const
{
  return m_intensity;
}

void csGL4PointLight::SetPosition(const csVector3f& position)
{
  m_position = position;
}

const csVector3f& csGL4PointLight::GetPosition() const
{
  return m_position;
}


void csGL4PointLight::SetRange(float range)
{
  m_range = range;
}


float csGL4PointLight::GetRange() const
{
  return m_range;
}

}
