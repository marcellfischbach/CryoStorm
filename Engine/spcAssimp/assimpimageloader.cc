
#include <spcAssimp/assimpimageloader.hh>
#include <iostream>
#include <assimp/Importer.hpp>
#include <spcCore/resource/vfs.hh>
#include <png.h>

namespace spc
{

AssimpImageLoader::AssimpImageLoader()
{
  SPC_CLASS_GEN_CONSTR;
}


bool AssimpImageLoader::CanLoad(const ResourceLocator& locator) const
{
  return true;
}

#define PNGSIGSIZE 8

iObject* AssimpImageLoader::Load(const ResourceLocator& locator) const
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

  std::cout << "Image: " << width << "x" << height << std::endl;

  return nullptr;
}

}