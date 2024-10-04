
#include <csOpenGL/gl4/csGL4Texture2D.hh>
#include <csOpenGL/gl4/csGL4PixelFormatMap.hh>
#include <csOpenGL/csGLError.hh>
#include <csCore/graphics/csImage.hh>
#include <csCore/graphics/iSampler.hh>
#include <csCore/math/csMath.hh>
#include <GL/glew.h>

namespace cryo::opengl
{
csGL4Texture2D::csGL4Texture2D()
  : iTexture2D(), m_target(GL_TEXTURE_2D), m_name(0), m_width(0), m_height(0), m_sampler(nullptr),
    m_multiSampling(false)
{
  CS_CLASS_GEN_CONSTR;
  glGenTextures(1, &m_name);
}

csGL4Texture2D::~csGL4Texture2D()
{
  glDeleteTextures(1, &m_name);
  m_name = 0;
}

void csGL4Texture2D::Bind()
{
  glBindTexture(m_target, m_name);
}

void csGL4Texture2D::Unbind()
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

bool csGL4Texture2D::Initialize(uint16_t width,
                                uint16_t height,
                                ePixelFormat format,
                                bool generateMipMaps,
                                uint16_t multiSamples)
{
  CS_GL_ERROR()
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

  CS_GL_ERROR()
  if (m_multiSampling)
  {
    glTexImage2DMultisample(
        GL_TEXTURE_2D_MULTISAMPLE,
        multiSamples,
        csGL4PixelFormatInternal[format],
        width,
        height,
        false);
  }
  else
  {
    glTexStorage2D(GL_TEXTURE_2D,
                   (GLsizei)m_level.size(),
                   csGL4PixelFormatSizedInternal[format],
                   width, height
    );
    CS_GL_ERROR()
  }


  return true;
}

uint16_t csGL4Texture2D::GetSamples() const
{
  return m_samples;
}


void csGL4Texture2D::Data(const csImage* image)
{

  for (uint16_t l = 0; l < image->GetNumberOfLayers(); l++)
  {
    Data(l, image->GetPixelFormat(), image->GetData(l));
  }
}

void csGL4Texture2D::Data(uint16_t level, const csImage* image)
{
  Data(level, image->GetPixelFormat(), image->GetData(level));
}

void csGL4Texture2D::Data(uint16_t level, ePixelFormat format, const void* data)
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
      csGL4PixelFormatClient[format],
      csGL4PixelFormatClientDataType[format],
      data
  );
}

void csGL4Texture2D::Data(uint16_t level,
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
      csGL4PixelFormatClient[format],
      csGL4PixelFormatClientDataType[format],
      data
  );
}

void csGL4Texture2D::SetSampler(iSampler* sampler)
{
  CS_SET(m_sampler, sampler);
}

iSampler* csGL4Texture2D::GetSampler()
{
  return m_sampler;
}

const iSampler* csGL4Texture2D::GetSampler() const
{
  return m_sampler;
}
ePixelFormat csGL4Texture2D::GetFormat() const
{
  return m_format;
}

}
