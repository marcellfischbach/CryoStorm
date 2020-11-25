
#include <spcCore/graphics/image.hh>
#include <spcCore/math/math.hh>

namespace spc
{


Image::Image(UInt16 width, UInt16 height, ePixelFormat format)
  : m_pixelFormat(format)
{
  SPC_CLASS_GEN_CONSTR;
  GenerateLayers(width, height);
  GenerateLayer(0);
}


void Image::GenerateLayers(UInt16 width, UInt16 height)
{
  // count the layers
  UInt16 w = width;
  UInt16 h = height;
  m_numberOfLayers = 0;
  do {
    m_numberOfLayers++;
    w = spcMax(w / 2, 1);
    h = spcMax(h / 2, 1);
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
    w = spcMax(w / 2, 1);
    h = spcMax(h / 2, 1);
    lc++;
  } while (w != 1 || h != 1);
}

UInt32 byte_per_pixel(ePixelFormat format)
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

void Image::GenerateLayer(UInt16 layer)
{
  if (layer >= m_numberOfLayers)
  {
    return;
  }

  UInt32 bpp = byte_per_pixel(m_pixelFormat);
  Layer& l = m_layers[layer];
  l.size = l.width * l.height * bpp;
  l.buffer = new UInt8[l.size];
}

void Image::GenerateMipMapLayers()
{
  for (UInt16 layer = 1; layer < m_numberOfLayers; layer++)
  {
    GenerateLayer(layer);
  }
}

void Image::Copy(UInt16 layer, const UInt8* buffer)
{
  if (layer >= m_numberOfLayers)
  {
    return;
  }

  if (!l.buffer)
  {
    GenerateLayer(layer);
  }

  memcpy(l.buffer, buffer, l.size);
}

UInt16 Image::GetNumberOfLayers() const
{
  return m_numberOfLayers;
}

UInt16 Image::GetWidth(UInt16 layer) const
{
  if (layer >= m_numberOfLayers)
  {
    return 0;
  }
  return m_layers[layer].width;
}

UInt16 Image::GetHeight(UInt16 layer) const
{
  if (layer >= m_numberOfLayers)
  {
    return 0;
  }
  return m_layers[layer].height;
}

ePixelFormat Image::GetPixelFormat() const
{
  return m_pixelFormat;
}


}