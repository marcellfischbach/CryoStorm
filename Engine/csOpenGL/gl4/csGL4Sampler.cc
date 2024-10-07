
#include <csOpenGL/gl4/csGL4Sampler.hh>
#include <csOpenGL/gl4/csGL4CompareFuncMap.hh>
#include <csOpenGL/gl4/csGL4TextureCompareModeMap.hh>
#include <csCore/math/csMath.hh>
#include <GL/glew.h>


namespace cs::opengl
{

csGL4Sampler::csGL4Sampler()
  : iSampler()
  , m_name(0)
  , m_filterMode(eFM_MinMagNearest)
  , m_anisotropy(1)
  , m_minLOD(-1000)
  , m_maxLOD(1000)
  , m_addressU(eTAM_Repeat)
  , m_addressV(eTAM_Repeat)
  , m_addressW(eTAM_Repeat)
  , m_borderColor(0, 0, 0, 0)
  , m_textureCompareMode(eTCM_None)
  , m_textureCompareFunc(eCF_Always)

{
  CS_CLASS_GEN_CONSTR;
  glGenSamplers(1, &m_name);
  glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glSamplerParameteri(m_name, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1);
  glSamplerParameteri(m_name, GL_TEXTURE_MIN_LOD, -1000);
  glSamplerParameteri(m_name, GL_TEXTURE_MAX_LOD, 1000);
  glSamplerParameteri(m_name, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glSamplerParameteri(m_name, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glSamplerParameteri(m_name, GL_TEXTURE_WRAP_R, GL_REPEAT);
  float borderColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
  glSamplerParameterfv(m_name, GL_TEXTURE_BORDER_COLOR, borderColor);
  glSamplerParameteri(m_name, GL_TEXTURE_COMPARE_MODE, GL_NONE);
  glSamplerParameteri(m_name, GL_TEXTURE_COMPARE_FUNC, GL_ALWAYS);

}

csGL4Sampler::~csGL4Sampler()
{
  glDeleteSamplers(1, &m_name);
  m_name = 0;
}

void csGL4Sampler::Bind(eTextureUnit unit)
{
  glBindSampler(unit, m_name);
}


void csGL4Sampler::Unbind(eTextureUnit unit)
{
  glBindSampler(unit, 0);
}


void csGL4Sampler::SetFilterMode(eFilterMode filterMode)
{
  if (m_filterMode != filterMode)
  {
    m_filterMode = filterMode;
    switch (m_filterMode)
    {
    case eFM_MinMagNearest:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      break;
    case eFM_MinNearestMagLinear:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      break;
    case eFM_MinLinearMagNearest:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      break;
    case eFM_MinMagLinear:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      break;
    case eFM_MinMagMipNearest:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      break;
    case eFM_MinMagNearestMipLinear:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      break;
    case eFM_MinNearestMagLinearMipNearest:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      break;
    case eFM_MinNearestMagMipLinear:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      SetAnisotropy(1);
      break;
    case eFM_MinLinearMagMipNearest:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      break;
    case eFM_MinLinearMagNearestMipLinear:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      break;
    case eFM_MinMagLinearMipNearest:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      break;
    case eFM_MinMagMipLinear:
    case eFM_Anisotropic:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      break;
    }
  }
}

eFilterMode csGL4Sampler::GetFilterMode() const
{
  return m_filterMode;
}

void csGL4Sampler::SetAnisotropy(uint8_t anisotropy)
{
  anisotropy = ceClamp<uint8_t>(anisotropy, 1, 16);
  if (m_anisotropy != anisotropy)
  {
    m_anisotropy = anisotropy;
    glSamplerParameteri(m_name, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_anisotropy);

  }
}

uint8_t csGL4Sampler::GetAnisotropy() const
{
  return m_anisotropy;
}

void csGL4Sampler::SetMinLOD(int16_t minLOD)
{
  if (m_minLOD != minLOD)
  {
    m_minLOD = minLOD;
    glSamplerParameteri(m_name, GL_TEXTURE_MIN_LOD, m_minLOD);
  }
}

int16_t csGL4Sampler::GetMinLOD() const
{
  return m_minLOD;
}

void csGL4Sampler::SetMaxLOD(int16_t maxLOD)
{
  if (m_maxLOD != maxLOD)
  {
    m_maxLOD = maxLOD;
    glSamplerParameteri(m_name, GL_TEXTURE_MAX_LOD, m_maxLOD);
  }
}

int16_t csGL4Sampler::GetMaxLOD() const
{
  return m_maxLOD;
}

static uint32_t Address(eTextureAddressMode tam)
{
  switch (tam)
  {
  case eTAM_Repeat: return GL_REPEAT;
  case eTAM_Clamp: return GL_CLAMP_TO_EDGE;
  case eTAM_Mirror: return GL_MIRRORED_REPEAT;
  case eTAM_MirrorOnce: return GL_MIRROR_CLAMP_TO_EDGE;
  }
  return GL_REPEAT;
}

void csGL4Sampler::SetAddressU(eTextureAddressMode mode)
{
  if (m_addressU != mode)
  {
    m_addressU = mode;
    glSamplerParameteri(m_name, GL_TEXTURE_WRAP_S, Address(mode));
  }
}


eTextureAddressMode csGL4Sampler::GetAddressU() const
{
  return m_addressU;
}

void csGL4Sampler::SetAddressV(eTextureAddressMode mode)
{
  if (m_addressV != mode)
  {
    m_addressV = mode;
    glSamplerParameteri(m_name, GL_TEXTURE_WRAP_T, Address(mode));
  }
}

eTextureAddressMode csGL4Sampler::GetAddressV() const
{
  return m_addressV;
}

void csGL4Sampler::SetAddressW(eTextureAddressMode mode)
{
  if (m_addressW != mode)
  {
    m_addressW = mode;
    glSamplerParameteri(m_name, GL_TEXTURE_WRAP_R, Address(mode));
  }
}

eTextureAddressMode csGL4Sampler::GetAddressW() const
{
  return m_addressW;
}

void csGL4Sampler::SetBorderColor(const csColor4f& borderColor)
{
  if (m_borderColor != borderColor)
  {
    m_borderColor = borderColor;
    glSamplerParameterfv(m_name, GL_TEXTURE_BORDER_COLOR, &m_borderColor.r);

  }
}

const csColor4f& csGL4Sampler::GetBorderColor() const
{
  return m_borderColor;
}

void csGL4Sampler::SetTextureCompareMode(eTextureCompareMode compareMode)
{
  if (m_textureCompareMode != compareMode)
  {
    m_textureCompareMode = compareMode;
    glSamplerParameteri(m_name, GL_TEXTURE_COMPARE_MODE, csGL4TextureCompareModeMap[m_textureCompareMode]);

  }
}

eTextureCompareMode csGL4Sampler::GetTextureCompareMode() const
{
  return m_textureCompareMode;
}

void csGL4Sampler::SetTextureCompareFunc(eCompareFunc compareFunc)
{
  if (m_textureCompareFunc != compareFunc)
  {
    m_textureCompareFunc = compareFunc;
    glSamplerParameteri(m_name, GL_TEXTURE_COMPARE_FUNC, csGL4CompareFuncMap[m_textureCompareFunc]);
  }
}

eCompareFunc csGL4Sampler::GetTextureCompareFunc() const
{
  return m_textureCompareFunc;
}

bool csGL4Sampler::NeedsMipMaps() const
{
  switch (m_filterMode)
  {
  case eFM_MinMagNearest:
  case eFM_MinNearestMagLinear:
  case eFM_MinLinearMagNearest:
  case eFM_MinMagLinear:
    return false;
  case eFM_MinMagMipNearest:
  case eFM_MinMagNearestMipLinear:
  case eFM_MinNearestMagLinearMipNearest:
  case eFM_MinNearestMagMipLinear:
  case eFM_MinLinearMagMipNearest:
  case eFM_MinLinearMagNearestMipLinear:
  case eFM_MinMagLinearMipNearest:
  case eFM_MinMagMipLinear:
  case eFM_Anisotropic:
    return true;
  }
  return false;
}

}
