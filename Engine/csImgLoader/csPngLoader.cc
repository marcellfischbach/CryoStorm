
#include <csImgLoader/csPngLoader.hh>
#include <iostream>
#include <csCore/resource/csVFS.hh>
#include <csCore/graphics/csImage.hh>
#include <png.h>

namespace cs::img
{

csPngLoader::csPngLoader()
{

  RegisterType("PNG");

}



#define PNGSIGSIZE 8


void read_data_from_i_file(png_structp png_ptr, png_bytep buffer, png_size_t size)
{
  png_voidp io_ptr = png_get_io_ptr(png_ptr);
  if (io_ptr == nullptr)
  {
    return; 
  }

  iFile* file = reinterpret_cast<iFile*>(io_ptr);
  file->Read(1, size, buffer);
}


iAsset *csPngLoader::Load(const csAssetLocator& locator) const
{
  iFile* fp = csVFS::Get()->Open(locator, eAM_Read, eOM_Binary);
  if (!fp)
  {
    return nullptr;
  }
  png_byte pngsig[PNGSIGSIZE];
  fp->Read(1, PNGSIGSIZE, pngsig);
  int is_png = png_sig_cmp(pngsig, 0, PNGSIGSIZE);
  if (is_png != 0)
  {
    std::cout << "Image: " + locator.Canonical() << " is no png" << std::endl;
    return nullptr;
  }


  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
  png_infop info_ptr = png_create_info_struct(png_ptr);
  setjmp(png_jmpbuf(png_ptr));

  png_set_read_fn(png_ptr, fp, read_data_from_i_file);
  png_set_sig_bytes(png_ptr, 8);
  png_read_info(png_ptr, info_ptr);

  uint32_t width = png_get_image_width(png_ptr, info_ptr);
  uint32_t height = png_get_image_height(png_ptr, info_ptr);
  uint8_t color_type = png_get_color_type(png_ptr, info_ptr);
  uint8_t bit_depth = png_get_bit_depth(png_ptr, info_ptr);

  ePixelFormat imageFormat;
  uint8_t bpp = 0;
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


  //Size stride = png_get_rowbytes(png_ptr, info_ptr);
  png_bytep* row_pointers = new png_bytep[height];
  for (uint32_t y = 0; y < height; y++)
  {
    row_pointers[y] = new png_byte[png_get_rowbytes(png_ptr, info_ptr)];
  }
  png_read_image(png_ptr, row_pointers);

  uint8_t* image_buffer = new uint8_t[width * height * bpp];
  uint8_t* iptr = image_buffer;

  //
  // convert row by row and delete the buffer
  for (uint32_t y = 0; y < height; y++)
  {
    Size rowId = y; // height - y - 1;
    png_bytep ptr = row_pointers[rowId];
    switch (color_type)
    {
    case PNG_COLOR_TYPE_GRAY:
      for (uint32_t x = 0; x < width; x++)
      {
        *iptr++ = ptr[x];
      }
      break;
    case PNG_COLOR_TYPE_GA:
      for (uint32_t x = 0; x < width; x++)
      {
        *iptr++ = ptr[x * 2 + 0];
        *iptr++ = ptr[x * 2 + 1];
      }
      break;
    case PNG_COLOR_TYPE_RGB:
      for (uint32_t x = 0; x < width; x++)
      {
        *iptr++ = ptr[x * 3 + 0];
        *iptr++ = ptr[x * 3 + 1];
        *iptr++ = ptr[x * 3 + 2];
      }
      break;
    case PNG_COLOR_TYPE_RGBA:
      for (uint32_t x = 0; x < width; x++)
      {
        *iptr++ = ptr[x * 4 + 0];
        *iptr++ = ptr[x * 4 + 1];
        *iptr++ = ptr[x * 4 + 2];
        *iptr++ = ptr[x * 4 + 3];
      }
      break;
    }

    delete[] row_pointers[rowId];
  }
  delete[] row_pointers;
  row_pointers = nullptr;

  csImage * img = new csImage(width, height, imageFormat);
  img->Copy(0, image_buffer);
  delete[] image_buffer;

  png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);


  fp->Close();
  return img;
}

}