
#include <csOpenGL/gl4/csGL4Texture2DArray.hh>
#include <csOpenGL/gl4/csGL4PixelFormatMap.hh>
#include <csCore/graphics/csImage.hh>
#include <csCore/graphics/iSampler.hh>
#include <csCore/math/csMath.hh>
#include <csOpenGL/csGLError.hh>
#include <GL/glew.h>

namespace cs::opengl
{
csGL4Texture2DArray::csGL4Texture2DArray()
  : iTexture2DArray()
  , m_width(0)
  , m_height(0)
  , m_layer(0)
  , m_sampler(nullptr)
{
  CS_CLASS_GEN_CONSTR;
  CS_GL_ERROR()
  glGenTextures(1, &m_name);
  CS_GL_ERROR()
}

csGL4Texture2DArray::~csGL4Texture2DArray()
{
  CS_GL_ERROR()
  glDeleteTextures(1, &m_name);
  CS_GL_ERROR()
  m_name = 0;
}

void csGL4Texture2DArray::Bind()
{
  CS_GL_ERROR()
  glBindTexture(GL_TEXTURE_2D_ARRAY, m_name);
  CS_GL_ERROR()
}

void csGL4Texture2DArray::Unbind()
{
  CS_GL_ERROR()
  glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
  CS_GL_ERROR()
}


bool csGL4Texture2DArray::Initialize(uint16_t width, uint16_t height, uint16_t layer, ePixelFormat format, bool generateMipMaps)
{
  m_width = width;
  m_height = height;
  m_layer = layer;
  m_format = format;
  Bind();

  CS_GL_ERROR()
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  CS_GL_ERROR()

  uint8_t level = 0;

  while (true)
  {
    Level lvl;
    lvl.Width = width;
    lvl.Height = height;
    m_level.push_back(lvl);

    CS_GL_ERROR()
    glTexImage3D(
        GL_TEXTURE_2D_ARRAY,
        level,
        csGL4PixelFormatInternal[format],
        width,
        height,
        m_layer,
        0,
        csGL4PixelFormatClient[format],
        csGL4PixelFormatClientDataType[format],
        nullptr
    );
    CS_GL_ERROR()
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

void csGL4Texture2DArray::Data(uint16_t layer, const csImage* image)
{
  for (uint16_t l=0; l<image->GetNumberOfLayers(); l++)
  {
    Data(layer, l, image->GetPixelFormat(), image->GetData(l));
  }
}

void csGL4Texture2DArray::Data(uint16_t layer, uint16_t level, const csImage* image)
{
  Data(layer, level, image->GetPixelFormat(), image->GetData(level));
}

void csGL4Texture2DArray::Data(uint16_t layer, uint16_t level, ePixelFormat format, const void* data)
{
  if (level >= m_level.size())
  {
    return;
  }


  Level& lvl = m_level[level];
  CS_GL_ERROR()
  glTexSubImage3D(
      GL_TEXTURE_2D_ARRAY,
      level,
      0, 0, layer,
      lvl.Width, lvl.Height, 1,
      csGL4PixelFormatClient[format],
      csGL4PixelFormatClientDataType[format],
      data
  );
  CS_GL_ERROR()
}

void csGL4Texture2DArray::Data(uint16_t layer, uint16_t level, uint16_t x, uint16_t y, uint16_t width, uint16_t height, ePixelFormat format, const void* data)
{
  if (level >= m_level.size())
  {
    return;
  }

  CS_GL_ERROR()
  glTexSubImage3D(
      GL_TEXTURE_2D_ARRAY,
      level,
      x, y, layer,
      width, height, 1,
      csGL4PixelFormatClient[format],
      csGL4PixelFormatClientDataType[format],
      data
  );
  CS_GL_ERROR()
}


void csGL4Texture2DArray::SetSampler(iSampler* sampler)
{
  CS_SET(m_sampler, sampler);
}

iSampler* csGL4Texture2DArray::GetSampler()
{
  return m_sampler;
}

const iSampler* csGL4Texture2DArray::GetSampler() const
{
  return m_sampler;
}
ePixelFormat csGL4Texture2DArray::GetFormat() const
{
  return m_format;
}

}
