
#include <spcPngLoader/pngloader.hh>
#include <iostream>
#include <spcCore/resource/vfs.hh>
#include <spcCore/graphics/image.hh>
#include <png.h>

namespace spc
{

PngLoader::PngLoader()
{
  SPC_CLASS_GEN_CONSTR;
}


bool PngLoader::CanLoad(const Class* cls, const ResourceLocator& locator) const
{
  if (cls->IsAssignableFrom<Image>())
  {
    return true;
  }
  return true;
}

#define PNGSIGSIZE 8



iObject* PngLoader::Load(const Class* cls, const ResourceLocator& locator) const
{
  FILE* fp = VFS::Get()->Open(locator, eAM_Read, eOM_Binary);
  if (!fp)
  {
    return nullptr;
  }

  png_byte pngsig[PNGSIGSIZE];
  fread(pngsig, 1, 8, fp);
  int is_png = png_sig_cmp(pngsig, 0, PNGSIGSIZE);
  if (is_png != 0)
  {
    std::cout << "Image: " + locator.GetLocator() << " is no png" << std::endl;
    return nullptr;
  }

  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
  png_infop info_ptr = png_create_info_struct(png_ptr);

  setjmp(png_jmpbuf(png_ptr));

  png_init_io(png_ptr, fp);
  png_set_sig_bytes(png_ptr, 8);
  png_read_info(png_ptr, info_ptr);

  UInt32 width = png_get_image_width(png_ptr, info_ptr);
  UInt32 height = png_get_image_height(png_ptr, info_ptr);
  UInt8 color_type = png_get_color_type(png_ptr, info_ptr);
  UInt8 bit_depth = png_get_bit_depth(png_ptr, info_ptr);
  std::cout << "Image: " << width << "x" << height << "  Color: " << std::hex << color_type << "   BPP: " << std::hex << bit_depth << std::endl;

  ePixelFormat imageFormat;
  UInt8 bpp = 0;
  switch (color_type)
  {
  case PNG_COLOR_TYPE_GRAY:
    bpp = 1;
    imageFormat = ePF_R;
    break;
  case PNG_COLOR_TYPE_GA:
    imageFormat = ePF_RG;
    bpp = 2;
    break;
  case PNG_COLOR_TYPE_RGB:
    imageFormat = ePF_RGB;
    bpp = 3;
    break;
  case PNG_COLOR_TYPE_RGBA:
    imageFormat = ePF_RGBA;
    bpp = 4;
    break;
  }


  UInt32 stride = png_get_rowbytes(png_ptr, info_ptr);
  png_bytep* row_pointers = new png_bytep[height];
  for (UInt32 y = 0; y < height; y++)
  {
    row_pointers[y] = new png_byte[png_get_rowbytes(png_ptr, info_ptr)];
  }
  png_read_image(png_ptr, row_pointers);

  UInt8* image_buffer = new UInt8[width * height * bpp];
  UInt8* iptr = image_buffer;

  //
  // convert row by row and delete the buffer
  for (UInt32 y = 0; y < height; y++)
  {
    png_bytep ptr = row_pointers[y];
    switch (color_type)
    {
    case PNG_COLOR_TYPE_GRAY:
      for (UInt32 x = 0; x < width; x++)
      {
        *iptr++ = ptr[x];
      }
      break;
    case PNG_COLOR_TYPE_GA:
      for (UInt32 x = 0; x < width; x++)
      {
        *iptr++ = ptr[x*2+0];
        *iptr++ = ptr[x*2+1];
      }
      break;
    case PNG_COLOR_TYPE_RGB:
      for (UInt32 x = 0; x < width; x++)
      {
        *iptr++ = ptr[x * 3 + 0];
        *iptr++ = ptr[x * 3 + 1];
        *iptr++ = ptr[x * 3 + 2];
      }
      break;
    case PNG_COLOR_TYPE_RGBA:
      for (UInt32 x = 0; x < width; x++)
      {
        *iptr++ = ptr[x * 4 + 0];
        *iptr++ = ptr[x * 4 + 1];
        *iptr++ = ptr[x * 4 + 2];
        *iptr++ = ptr[x * 4 + 3];
      }
      break;
    }

    delete[] row_pointers[y];
  }
  delete[] row_pointers;
  row_pointers = nullptr;

  Image* img = new Image(width, height, imageFormat);
  img->Copy(0, image_buffer);
  delete[] image_buffer;

  return img;
}

}