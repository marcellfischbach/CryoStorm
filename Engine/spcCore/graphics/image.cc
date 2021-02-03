
#include <spcCore/graphics/image.hh>
#include <spcCore/math/math.hh>
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

void Image::GenerateMipMaps(eMipMapProcedure procedure)
{
  GenerateMipMapLayers();
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
  Layer& l = m_layers[layer];
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

const UInt8* Image::GetData(UInt16 layer) const
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
  for (UInt16 l = 1; l<m_numberOfLayers; ++l)
  {
    Layer& src = m_layers[l-1];
    Layer& dst = m_layers[l];

    UInt32 bpp = byte_per_pixel(m_pixelFormat);
    UInt8* dptr = dst.buffer;
    for (int y=0; y<dst.height; y++)
    {
      UInt32 srcRow0Idx = spcMin(y * 2, src.height-1) * src.width * bpp;
      UInt32 srcRow1Idx = spcMin(y * 2 + 1, src.height-1) * src.width * bpp;
      for (int x=0; x<dst.width; x++)
      {
        UInt32 src00Idx = srcRow0Idx + spcMin(x * 2, src.width-1) * bpp;
        UInt32 src01Idx = srcRow0Idx + spcMin(x * 2 + 1, src.width-1) * bpp;
        UInt32 src10Idx = srcRow1Idx + spcMin(x * 2, src.width-1) * bpp;
        UInt32 src11Idx = srcRow1Idx + spcMin(x * 2 + 1, src.width-1) * bpp;
        for (UInt32 c = 0; c<bpp; c++)
        {
          UInt32 s = src.buffer[src00Idx+c]
                  + src.buffer[src01Idx+c]
                  + src.buffer[src10Idx+c]
                  + src.buffer[src11Idx+c];
          s >>= 2;
          *dptr++ = (UInt8)(s & 0x000000ff);
        }
      }
    }
  }
}

void Image::GenerateMipMapsNormal()
{
  for (UInt16 l = 1; l<m_numberOfLayers; ++l)
  {
    Layer& src = m_layers[l-1];
    Layer& dst = m_layers[l];

    UInt32 bpp = byte_per_pixel(m_pixelFormat);
    UInt8* dptr = dst.buffer;
    for (int y=0; y<dst.width; y++)
    {
      UInt32 srcRow0Idx = spcMin(y * 2, src.height-1) * src.width * bpp;
      UInt32 srcRow1Idx = spcMin(y * 2 + 1, src.height-1) * src.width * bpp;
      for (int x=0; x<dst.width; x++)
      {
        UInt32 src00Idx = srcRow0Idx + spcMin(x * 2, src.width-1) * bpp;
        UInt32 src01Idx = srcRow0Idx + spcMin(x * 2 + 1, src.width-1) * bpp;
        UInt32 src10Idx = srcRow1Idx + spcMin(x * 2, src.width-1) * bpp;
        UInt32 src11Idx = srcRow1Idx + spcMin(x * 2 + 1, src.width-1) * bpp;
        for (UInt32 c = 0; c<bpp; c++)
        {
          UInt32 s = src.buffer[src00Idx+c]
                     + src.buffer[src01Idx+c]
                     + src.buffer[src10Idx+c]
                     + src.buffer[src11Idx+c];
          s >> 2;
          *dptr++ = (UInt8)(s & 0x000000ff);
        }
      }
    }
  }
}

}