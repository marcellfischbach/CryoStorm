
#include <ceOpenGL/gl4/gl4texture2d.hh>
#include <ceOpenGL/gl4/gl4pixelformatmap.hh>
#include <ceOpenGL/glerror.hh>
#include <ceCore/graphics/image.hh>
#include <ceCore/graphics/isampler.hh>
#include <ceCore/math/math.hh>
#include <GL/glew.h>

namespace ce::opengl
{
GL4Texture2D::GL4Texture2D()
  : iTexture2D(), m_target(GL_TEXTURE_2D), m_name(0), m_width(0), m_height(0), m_sampler(nullptr),
    m_multiSampling(false)
{
  CE_CLASS_GEN_CONSTR;
  glGenTextures(1, &m_name);
}

GL4Texture2D::~GL4Texture2D()
{
  glDeleteTextures(1, &m_name);
  m_name = 0;
}

void GL4Texture2D::Bind()
{
  glBindTexture(m_target, m_name);
}

void GL4Texture2D::Unbind()
{
  glBindTexture(m_target, 0);
}

static size_t number_of_layers(uint16_t width, uint16_t height)
{
  size_t count = 0;
  while (true)
  {
    count++;
    if (width == 1 && height == 1)
    {
      break;
    }
    width  = width / 2;
    height = height / 2;
    width  = ceMax(width, (uint16_t)1);
    height = ceMax(height, (uint16_t)1);
  }
  return count;
}

bool GL4Texture2D::Initialize(uint16_t width,
                              uint16_t height,
                              ePixelFormat format,
                              bool generateMipMaps,
                              uint16_t multiSamples)
{
  CE_GL_ERROR()
  m_multiSampling = multiSamples > 1;
  m_target        = m_multiSampling ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
  m_width         = width;
  m_height        = height;
  m_format        = format;
  m_samples       = multiSamples;
  Bind();

  if (!m_multiSampling)
  {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  }

  uint16_t lvlWidth  = width;
  uint16_t lvlHeight = height;
  while (true)
  {
    Level lvl;
    lvl.Width  = lvlWidth;
    lvl.Height = lvlHeight;
    m_level.push_back(lvl);

    if (!generateMipMaps || m_multiSampling || lvlWidth == 1 && lvlHeight == 1)
    {
      break;
    }

    lvlWidth  = ceMax(lvlWidth / 2, 1);
    lvlHeight = ceMax(lvlHeight / 2, 1);
  }

  CE_GL_ERROR()
  if (m_multiSampling)
  {
    glTexImage2DMultisample(
      GL_TEXTURE_2D_MULTISAMPLE,
      multiSamples,
      GL4PixelFormatInternal[format],
      width,
      height,
      false);
  }
  else
  {
    glTexStorage2D(GL_TEXTURE_2D,
                   (GLsizei)m_level.size(),
                   GL4PixelFormatSizedInternal[format],
                   width, height
    );
    CE_GL_ERROR()
  }


  return true;
}

uint16_t GL4Texture2D::GetSamples() const
{
  return m_samples;
}


void GL4Texture2D::Data(const Image* image)
{

  for (uint16_t l = 0; l < image->GetNumberOfLayers(); l++)
  {
    Data(l, image->GetPixelFormat(), image->GetData(l));
  }
}

void GL4Texture2D::Data(uint16_t level, const Image* image)
{
  Data(level, image->GetPixelFormat(), image->GetData(level));
}

void GL4Texture2D::Data(uint16_t level, ePixelFormat format, const void* data)
{
  if (m_multiSampling || level >= m_level.size())
  {
    return;
  }


  Level& lvl = m_level[level];
  glTexSubImage2D(
    GL_TEXTURE_2D,
    level,
    0, 0,
    lvl.Width, lvl.Height,
    GL4PixelFormatClient[format],
    GL4PixelFormatClientDataType[format],
    data
  );
}

void GL4Texture2D::Data(uint16_t level,
                        uint16_t x,
                        uint16_t y,
                        uint16_t width,
                        uint16_t height,
                        ePixelFormat format,
                        const void* data)
{
  if (m_multiSampling || level >= m_level.size())
  {
    return;
  }

  glTexSubImage2D(
    GL_TEXTURE_2D,
    level,
    x, y,
    width, height,
    GL4PixelFormatClient[format],
    GL4PixelFormatClientDataType[format],
    data
  );
}

void GL4Texture2D::SetSampler(iSampler* sampler)
{
  CE_SET(m_sampler, sampler);
}

iSampler* GL4Texture2D::GetSampler()
{
  return m_sampler;
}

const iSampler* GL4Texture2D::GetSampler() const
{
  return m_sampler;
}
ePixelFormat GL4Texture2D::GetFormat() const
{
  return m_format;
}

}
