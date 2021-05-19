
#include <spcOpenGL/gl4/gl4directionallight.hh>


namespace spc::opengl
{

GL4DirectionalLight::GL4DirectionalLight()
  : iDirectionalLight()
  , m_changeMode(eLCM_Dynamic)
  , m_castShadow(true)
  , m_shadowBias(0.001f)
  , m_color(1.0f, 1.0f, 1.0f)
  , m_intensity(1.0f)
  , m_split(10.0f, 25.0f, 100.0f)
  , m_direction(0.0f, 0.0f, 1.0f)
{
  SPC_CLASS_GEN_CONSTR;
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

void GL4DirectionalLight::SetShadowMapBias(float shadowBias)
{
  m_shadowBias = shadowBias;
}

float GL4DirectionalLight::GetShadowMapBias() const
{
  return m_shadowBias;
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

void GL4DirectionalLight::SetIntensity(float intensity)
{
  m_intensity = intensity;
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

void GL4DirectionalLight::SetSplits(float split0, float split1, float split2) 
{
  if (split0 < split1 && split1 < split2)
  {
    m_split = Vector3f(split0, split1, split2);
  }
}

float GL4DirectionalLight::GetSplit0() const
{
  return m_split.x;
}


float GL4DirectionalLight::GetSplit1() const
{
  return m_split.y;
}


float GL4DirectionalLight::GetSplit2() const
{
  return m_split.z;
}

}
