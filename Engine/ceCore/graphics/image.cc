
#include <ceCore/graphics/image.hh>
#include <ceCore/math/math.hh>
#include <string.h>

namespace ce
{


Image::Image(uint16_t width, uint16_t height, ePixelFormat format)
  : m_pixelFormat(format)
  , m_mipmapCreated(false)
{
  CE_CLASS_GEN_CONSTR;
  GenerateLayers(width, height);
  GenerateLayer(0);
}


void Image::GenerateLayers(uint16_t width, uint16_t height)
{
  // count the layers
  uint16_t w = width;
  uint16_t h = height;
  m_numberOfLayers = 0;
  do {
    m_numberOfLayers++;
    w = ceMax(w / 2, 1);
    h = ceMax(h / 2, 1);
  } while (w != 1 || h != 1);
  m_layers = new Layer[m_numberOfLayers];
  memset(m_layers, 0, sizeof(Layer) * m_numberOfLayers);

  w = width;
  h = height;
  int lc = 0;
  do {
    Layer& layer = m_layers[lc];
    layer.width = w;
    layer.height = h;
    layer.buffer = nullptr;
    w = ceMax(w / 2, 1);
    h = ceMax(h / 2, 1);
    lc++;
  } while (w != 1 || h != 1);
}

uint32_t byte_per_pixel(ePixelFormat format)
{
  switch (format)
  {
  case ePF_RGB:
    return 3;
  case ePF_RGBA:
    return 4;
  }
  return 0;
}

void Image::GenerateLayer(uint16_t layer)
{
  if (layer >= m_numberOfLayers)
  {
    return;
  }

  uint32_t bpp = byte_per_pixel(m_pixelFormat);
  Layer& l = m_layers[layer];
  l.size = l.width * l.height * bpp;
  l.buffer = new uint8_t[l.size];
}

void Image::GenerateMipMaps(eMipMapProcedure procedure)
{
  if (!m_mipmapCreated)
  {
    GenerateMipMapLayers();
  }

  if (!m_mipmapCreated || m_mipMapProcedure != procedure)
  {
    switch (procedure)
    {
      case eMipMapProcedure::eMMP_Linear4x4:
        GenerateMipMapsLinear4x4();
        break;
      case eMipMapProcedure::eMMP_Normal:
        GenerateMipMapsNormal();
        break;
    }
  }

  m_mipmapCreated = true;
  m_mipMapProcedure = procedure;
}

void Image::GenerateMipMapLayers()
{
  for (uint16_t layer = 1; layer < m_numberOfLayers; layer++)
  {
    GenerateLayer(layer);
  }
}

void Image::Copy(uint16_t layer, const uint8_t* buffer)
{
  if (layer >= m_numberOfLayers)
  {
    return;
  }
  Layer& l = m_layers[layer];
  if (!l.buffer)
  {
    GenerateLayer(layer);
  }

  memcpy(l.buffer, buffer, l.size);
}

uint16_t Image::GetNumberOfLayers() const
{
  return m_numberOfLayers;
}

uint16_t Image::GetWidth(uint16_t layer) const
{
  if (layer >= m_numberOfLayers)
  {
    return 0;
  }
  return m_layers[layer].width;
}

uint16_t Image::GetHeight(uint16_t layer) const
{
  if (layer >= m_numberOfLayers)
  {
    return 0;
  }
  return m_layers[layer].height;
}

const uint8_t* Image::GetData(uint16_t layer) const
{
  if (layer >= m_numberOfLayers)
  {
    return nullptr;
  }
  return m_layers[layer].buffer;
}


ePixelFormat Image::GetPixelFormat() const
{
  return m_pixelFormat;
}


void Image::GenerateMipMapsLinear4x4()
{
  for (uint16_t l = 1; l<m_numberOfLayers; ++l)
  {
    Layer& src = m_layers[l-1];
    Layer& dst = m_layers[l];

    uint32_t bpp = byte_per_pixel(m_pixelFormat);
    uint8_t* dptr = dst.buffer;
    for (int y=0; y<dst.height; y++)
    {
      uint32_t srcRow0Idx = ceMin(y * 2, src.height-1) * src.width * bpp;
      uint32_t srcRow1Idx = ceMin(y * 2 + 1, src.height-1) * src.width * bpp;
      for (int x=0; x<dst.width; x++)
      {
        uint32_t src00Idx = srcRow0Idx + ceMin(x * 2, src.width-1) * bpp;
        uint32_t src01Idx = srcRow0Idx + ceMin(x * 2 + 1, src.width-1) * bpp;
        uint32_t src10Idx = srcRow1Idx + ceMin(x * 2, src.width-1) * bpp;
        uint32_t src11Idx = srcRow1Idx + ceMin(x * 2 + 1, src.width-1) * bpp;
        for (uint32_t c = 0; c<bpp; c++)
        {
          uint32_t s = src.buffer[src00Idx+c]
                  + src.buffer[src01Idx+c]
                  + src.buffer[src10Idx+c]
                  + src.buffer[src11Idx+c];
          s >>= 2;
          *dptr++ = (uint8_t)(s & 0x000000ff);
        }
      }
    }
  }
}

void Image::GenerateMipMapsNormal()
{
  for (uint16_t l = 1; l<m_numberOfLayers; ++l)
  {
    Layer& src = m_layers[l-1];
    Layer& dst = m_layers[l];

    uint32_t bpp = byte_per_pixel(m_pixelFormat);
    uint8_t* dptr = dst.buffer;
    for (int y=0; y<dst.height; y++)
    {
      uint32_t srcRow0Idx = ceMin(y * 2, src.height-1) * src.width * bpp;
      uint32_t srcRow1Idx = ceMin(y * 2 + 1, src.height-1) * src.width * bpp;
      for (int x=0; x<dst.width; x++)
      {
        uint32_t src00Idx = srcRow0Idx + ceMin(x * 2, src.width-1) * bpp;
        uint32_t src01Idx = srcRow0Idx + ceMin(x * 2 + 1, src.width-1) * bpp;
        uint32_t src10Idx = srcRow1Idx + ceMin(x * 2, src.width-1) * bpp;
        uint32_t src11Idx = srcRow1Idx + ceMin(x * 2 + 1, src.width-1) * bpp;
        for (uint32_t c = 0; c<bpp; c++)
        {
          uint32_t s = src.buffer[src00Idx+c]
                     + src.buffer[src01Idx+c]
                     + src.buffer[src10Idx+c]
                     + src.buffer[src11Idx+c];
          s >>= 2;
          *dptr++ = (uint8_t)(s & 0x000000ff);
        }
      }
    }
  }
}

}