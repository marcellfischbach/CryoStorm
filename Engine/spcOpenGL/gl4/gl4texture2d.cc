
#include <spcOpenGL/gl4/gl4texture2d.hh>
#include <spcOpenGL/gl4/gl4pixelformatmap.hh>
#include <spcCore/graphics/image.hh>
#include <spcCore/graphics/isampler.hh>
#include <spcCore/math/math.hh>
#include <GL/glew.h>

namespace spc::opengl
{
GL4Texture2D::GL4Texture2D()
  : iTexture2D()
  , m_width(0)
  , m_height(0)
  , m_sampler(nullptr)
{
  SPC_CLASS_GEN_CONSTR;
  glGenTextures(1, &m_name);
}

GL4Texture2D::~GL4Texture2D()
{
  glDeleteTextures(1, &m_name);
  m_name = 0;
}

void GL4Texture2D::Bind()
{
  glBindTexture(GL_TEXTURE_2D, m_name);
}

bool GL4Texture2D::Initialize(UInt16 width, UInt16 height, ePixelFormat format, bool generateMipMaps)
{
  m_width = width;
  m_height = height;
  Bind();

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  UInt8 level = 0;

  while (true)
  {
    Level lvl;
    lvl.Width = width;
    lvl.Height = height;
    m_level.push_back(lvl);

    glTexImage2D(
      GL_TEXTURE_2D,
      level,
      GL4PixelFormatInternal[format],
      width,
      height,
      0,
      GL4PixelFormatClient[format],
      GL4PixelFormatClientDataType[format],
      nullptr
    );
    if (!generateMipMaps || width == 1 && height == 1)
    {
      break;
    }

    width = spcMax(width / 2, 1);
    height = spcMax(height / 2, 1);
    level++;
  }
  return true;
}

void GL4Texture2D::Data(const Image* image)
{
  for (UInt16 l=0; l<image->GetNumberOfLayers(); l++)
  {
    Data(l, image->GetPixelFormat(), image->GetData(l));
  }
}

void GL4Texture2D::Data(UInt8 level, const Image* image)
{
  Data(level, image->GetPixelFormat(), image->GetData(level));
}

void GL4Texture2D::Data(UInt8 level, ePixelFormat format, const void* data)
{
  if (level >= m_level.size())
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

void GL4Texture2D::Data(UInt8 level, UInt16 x, UInt16 y, UInt16 width, UInt16 height, ePixelFormat format, const void* data)
{
  if (level >= m_level.size())
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
  SPC_SET(m_sampler, sampler);
}

iSampler* GL4Texture2D::GetSampler()
{
  return m_sampler;
}

const iSampler* GL4Texture2D::GetSampler() const
{
  return m_sampler;
}

}
