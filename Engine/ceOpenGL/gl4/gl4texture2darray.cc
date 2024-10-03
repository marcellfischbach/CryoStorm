
#include <ceOpenGL/gl4/gl4texture2darray.hh>
#include <ceOpenGL/gl4/gl4pixelformatmap.hh>
#include <ceCore/graphics/csImage.hh>
#include <ceCore/graphics/iSampler.hh>
#include <ceCore/math/math.hh>
#include <GL/glew.h>

namespace cryo::opengl
{
GL4Texture2DArray::GL4Texture2DArray()
  : iTexture2DArray()
  , m_width(0)
  , m_height(0)
  , m_layer(0)
  , m_sampler(nullptr)
{
  CS_CLASS_GEN_CONSTR;
  glGenTextures(1, &m_name);
}

GL4Texture2DArray::~GL4Texture2DArray()
{
  glDeleteTextures(1, &m_name);
  m_name = 0;
}

void GL4Texture2DArray::Bind()
{
  glBindTexture(GL_TEXTURE_2D_ARRAY, m_name);
}

void GL4Texture2DArray::Unbind()
{
  glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}


bool GL4Texture2DArray::Initialize(uint16_t width, uint16_t height, uint16_t layer, ePixelFormat format, bool generateMipMaps)
{
  m_width = width;
  m_height = height;
  m_layer = layer;
  m_format = format;
  Bind();

  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  uint8_t level = 0;

  while (true)
  {
    Level lvl;
    lvl.Width = width;
    lvl.Height = height;
    m_level.push_back(lvl);

    glTexImage3D(
      GL_TEXTURE_2D_ARRAY,
      level,
      GL4PixelFormatInternal[format],
      width,
      height,
      m_layer,
      0,
      GL4PixelFormatClient[format],
      GL4PixelFormatClientDataType[format],
      nullptr
    );
    if (!generateMipMaps || width == 1 && height == 1)
    {
      break;
    }

    width = ceMax(width / 2, 1);
    height = ceMax(height / 2, 1);
    level++;
  }
  return true;
}

void GL4Texture2DArray::Data(uint16_t layer, const csImage* image)
{
  for (uint16_t l=0; l<image->GetNumberOfLayers(); l++)
  {
    Data(layer, l, image->GetPixelFormat(), image->GetData(l));
  }
}

void GL4Texture2DArray::Data(uint16_t layer, uint16_t level, const csImage* image)
{
  Data(layer, level, image->GetPixelFormat(), image->GetData(level));
}

void GL4Texture2DArray::Data(uint16_t layer, uint16_t level, ePixelFormat format, const void* data)
{
  if (level >= m_level.size())
  {
    return;
  }


  Level& lvl = m_level[level];
  glTexSubImage3D(
    GL_TEXTURE_2D_ARRAY,
    level,
    0, 0, layer,
    lvl.Width, lvl.Height, 1,
    GL4PixelFormatClient[format],
    GL4PixelFormatClientDataType[format],
    data
  );
}

void GL4Texture2DArray::Data(uint16_t layer, uint16_t level, uint16_t x, uint16_t y, uint16_t width, uint16_t height, ePixelFormat format, const void* data)
{
  if (level >= m_level.size())
  {
    return;
  }

  glTexSubImage3D(
    GL_TEXTURE_2D_ARRAY,
    level,
    x, y, layer,
    width, height, 1,
    GL4PixelFormatClient[format],
    GL4PixelFormatClientDataType[format],
    data
  );
}


void GL4Texture2DArray::SetSampler(iSampler* sampler)
{
  CS_SET(m_sampler, sampler);
}

iSampler* GL4Texture2DArray::GetSampler()
{
  return m_sampler;
}

const iSampler* GL4Texture2DArray::GetSampler() const
{
  return m_sampler;
}
ePixelFormat GL4Texture2DArray::GetFormat() const
{
  return m_format;
}

}
