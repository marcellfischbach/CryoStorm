
#include <csOpenGL/gl4/csGL4TextureCube.hh>
#include <csOpenGL/gl4/csGL4PixelFormatMap.hh>
#include <csOpenGL/csGLError.hh>
#include <csCore/graphics/csImage.hh>
#include <csCore/graphics/iSampler.hh>
#include <csCore/math/csMath.hh>
#include <GL/glew.h>

namespace cs::opengl
{

csGL4TextureCube::csGL4TextureCube()
        : iTextureCube()
        , m_size(0)
        , m_sampler(nullptr)
{
  CS_GL_ERROR()
  glGenTextures(1, &m_name);
  CS_GL_ERROR()
}

csGL4TextureCube::~csGL4TextureCube()
{
  CS_GL_ERROR()
  glDeleteTextures(1, &m_name);
  CS_GL_ERROR()
  m_name = 0;
}

void csGL4TextureCube::Bind() const
{
  CS_GL_ERROR()
  glBindTexture(GL_TEXTURE_CUBE_MAP, m_name);
  CS_GL_ERROR()
}

void csGL4TextureCube::Unbind() const
{
  CS_GL_ERROR()
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  CS_GL_ERROR()
}

bool csGL4TextureCube::Initialize(uint16_t size, ePixelFormat format, bool generateMipMaps)
{
  m_size = size;
  m_format = format;
  Bind();

  CS_GL_ERROR()
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  CS_GL_ERROR()

  uint8_t level = 0;

  while (true)
  {
    Level lvl;
    lvl.Size = size;
    m_level.push_back(lvl);
    for (GLenum i=0; i<6; i++)
    {
      CS_GL_ERROR()
      glTexImage2D(
              GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
              level,
              csGL4PixelFormatInternal[format],
              size,
              size,
              0,
              csGL4PixelFormatClient[format],
              csGL4PixelFormatClientDataType[format],
              nullptr
      );
      CS_GL_ERROR()
    }
    if (!generateMipMaps || size == 1)
    {
      break;
    }

    size = ceMax(size / 2, 1);
    level++;
  }
  return true;
}

void csGL4TextureCube::Data(eCubeFace face, const csImage* image)
{
  for (uint16_t l=0; l<image->GetNumberOfLayers(); l++)
  {
    Data(face, l, image->GetPixelFormat(), image->GetData(l));
  }
}

void csGL4TextureCube::Data(eCubeFace face, uint16_t level, const csImage* image)
{
  Data(face, level, image->GetPixelFormat(), image->GetData(level));
}

void csGL4TextureCube::Data(eCubeFace face, uint16_t level, ePixelFormat format, const void* data)
{
  if (level >= m_level.size())
  {
    return;
  }


  Level& lvl = m_level[level];
  CS_GL_ERROR()
  glTexSubImage2D(
          GL_TEXTURE_CUBE_MAP_POSITIVE_X + static_cast<GLenum>(face),
          level,
          0, 0,
          lvl.Size, lvl.Size,
          csGL4PixelFormatClient[format],
          csGL4PixelFormatClientDataType[format],
          data
  );
  CS_GL_ERROR()
}

void csGL4TextureCube::Data(eCubeFace face, uint16_t level, uint16_t x, uint16_t y, uint16_t width, uint16_t height, ePixelFormat format, const void* data)
{
  if (level >= m_level.size())
  {
    return;
  }

  CS_GL_ERROR()
  glTexSubImage2D(
          GL_TEXTURE_CUBE_MAP_POSITIVE_X + static_cast<GLenum>(face),
          level,
          x, y,
          width, height,
          csGL4PixelFormatClient[format],
          csGL4PixelFormatClientDataType[format],
          data
  );
  CS_GL_ERROR()
}


void csGL4TextureCube::SetSampler(iSampler* sampler)
{
  m_sampler = sampler;
}

iSampler* csGL4TextureCube::GetSampler()
{
  return m_sampler;
}

const iSampler* csGL4TextureCube::GetSampler() const
{
  return m_sampler;
}
ePixelFormat csGL4TextureCube::GetFormat() const
{
  return m_format;
}

}
