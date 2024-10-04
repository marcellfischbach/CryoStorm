
#include <csOpenGL/gl4/csGL4DirectionalLight.hh>


namespace cryo::opengl
{

csGL4DirectionalLight::csGL4DirectionalLight()
  : iDirectionalLight()
  , m_changeMode(eLCM_Dynamic)
  , m_castShadow(true)
  , m_shadowBias(0.001f)
  , m_color(1.0f, 1.0f, 1.0f)
  , m_intensity(1.0f)
  , m_direction(0.0f, 0.0f, 1.0f)
{
  CS_CLASS_GEN_CONSTR;
}



void csGL4DirectionalLight::SetChangeMode(eLightChangeMode changeMode)
{
  m_changeMode = changeMode;
}

eLightChangeMode csGL4DirectionalLight::GetChangeMode() const
{
  return m_changeMode;
}

void csGL4DirectionalLight::SetCastShadow(bool castShadow)
{
  m_castShadow = castShadow;
}

bool csGL4DirectionalLight::IsCastShadow() const
{
  return m_castShadow;
}

void csGL4DirectionalLight::SetShadowMapBias(float shadowBias)
{
  m_shadowBias = shadowBias;
}

float csGL4DirectionalLight::GetShadowMapBias() const
{
  return m_shadowBias;
}

void csGL4DirectionalLight::SetColor(const csColor4f& color)
{
  m_color = color;
  m_intensity = color.r + color.g + color.b;
}

const csColor4f& csGL4DirectionalLight::GetColor() const
{
  return m_color;
}

void csGL4DirectionalLight::SetIntensity(float intensity)
{
  m_intensity = intensity;
}

float csGL4DirectionalLight::GetIntensity() const
{
  return m_intensity;
}

void csGL4DirectionalLight::SetDirection(const csVector3f& direction)
{
  m_direction = direction;
}

const csVector3f& csGL4DirectionalLight::GetDirection() const
{
  return m_direction;
}

}
