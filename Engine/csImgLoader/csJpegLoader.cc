
#include <csImgLoader/csJpegLoader.hh>
#include <iostream>
#include <csCore/resource/csVFS.hh>
#include <csCore/graphics/csImage.hh>
#include <csCore/resource/csFileSystemFile.hh>

extern "C" {
#ifdef CS_WIN32
//#include <Windows.h>
#endif

#include <jpeglib.h>
}

namespace cs::img
{


struct my_error_mgr
{
  struct jpeg_error_mgr pub;  /* "public" fields */

  jmp_buf setjmp_buffer;  /* for return to caller */
};


// Structure to hold memory source
struct MemorySourceManager
{
  struct jpeg_source_mgr pub;
  const unsigned char *buffer;
  size_t buffer_size;
  bool   start_of_file;
};

// Initialize source
void init_source(j_decompress_ptr cinfo)
{
  auto *src = (MemorySourceManager *) cinfo->src;
  src->start_of_file = true;
}

// Fill input buffer
boolean fill_input_buffer(j_decompress_ptr cinfo)
{
  auto *src = (MemorySourceManager *) cinfo->src;

  // Add an End-Of-Image (EOI) marker to the buffer
  static const JOCTET eoi_buffer[2] = {0xFF, JPEG_EOI};
  src->pub.next_input_byte = eoi_buffer;
  src->pub.bytes_in_buffer = 2;

  return TRUE;
}

// Skip data
void skip_input_data(j_decompress_ptr cinfo, long num_bytes)
{
  auto *src = (MemorySourceManager *) cinfo->src;

  if (num_bytes > 0)
  {
    if (num_bytes < (long) src->pub.bytes_in_buffer)
    {
      src->pub.next_input_byte += num_bytes;
      src->pub.bytes_in_buffer -= num_bytes;
    }
    else
    {
      fill_input_buffer(cinfo);
    }
  }
}

// Terminate source
void term_source(j_decompress_ptr cinfo)
{
  // No-op
}

// Set up the custom source manager
void jpeg_mem_src(j_decompress_ptr cinfo, const unsigned char *buffer, size_t buffer_size)
{
  if (cinfo->src == nullptr)
  {
    cinfo->src = (struct jpeg_source_mgr *) (*cinfo->mem->alloc_small)(
        (j_common_ptr) cinfo, JPOOL_PERMANENT, sizeof(MemorySourceManager));
  }

  auto *src = (MemorySourceManager *) cinfo->src;
  src->buffer      = buffer;
  src->buffer_size = buffer_size;

  src->pub.init_source       = init_source;
  src->pub.fill_input_buffer = fill_input_buffer;
  src->pub.skip_input_data   = skip_input_data;
  src->pub.resync_to_restart = jpeg_resync_to_restart; // Default method
  src->pub.term_source       = term_source;

  src->pub.bytes_in_buffer = buffer_size;
  src->pub.next_input_byte = buffer;
}


csJpegLoader::csJpegLoader()
{
  RegisterType("JPEG");
  RegisterType("JPG");


}


csOwned<iAsset> csJpegLoader::Load(const csAssetLocator &locator) const
{
  csRef<iFile> ifile = cs::csVFS::Get()->Open(locator, eAM_Read, eOM_Binary);
  if (!ifile)
  {
    return nullptr;
  }

  std::vector<char> memBuffer = ifile->ReadAll();


  struct jpeg_decompress_struct cinfo;

  struct my_error_mgr jerr;
  cinfo.err = jpeg_std_error(&jerr.pub);

  if (setjmp(jerr.setjmp_buffer))
  {
    /* If we get here, the JPEG code has signaled an error.
     * We need to clean up the JPEG object, close the input file, and return.
     */
    jpeg_destroy_decompress(&cinfo);
    return 0;
  }

  /* Now we can initialize the JPEG decompression object. */
  jpeg_create_decompress(&cinfo);

  jpeg_mem_src(&cinfo, (unsigned char*)memBuffer.data(), memBuffer.size());

  jpeg_read_header(&cinfo, TRUE);
  jpeg_start_decompress(&cinfo);

  int        row_stride = cinfo.output_width * cinfo.output_components;
  JSAMPARRAY buffer     = (*cinfo.mem->alloc_sarray)
      ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

  uint8_t  *image_buffer = new uint8_t[row_stride * cinfo.output_height];
  uint16_t row           = 0;
  while (cinfo.output_scanline < cinfo.output_height)
  {
    /* jpeg_read_scanlines expects an array of pointers to scanlines.
     * Here the array is only one element long, but you could ask for
     * more than one scanline at a time if that's more convenient.
     */
    (void) jpeg_read_scanlines(&cinfo, buffer, 1);
    /* Assume put_scanline_someplace wants a pointer and sample count. */
    //put_scanline_someplace(buffer[0], row_stride);
    memcpy(&image_buffer[(cinfo.output_height - row - 1) * row_stride], buffer[0], row_stride);
    row++;
  }

  ePixelFormat imageFormat = ePF_RGB;
  switch (cinfo.output_components)
  {
    case 1:
      imageFormat = ePF_R;
      break;
    case 2:
      imageFormat = ePF_RG;
      break;
    case 3:
      imageFormat = ePF_RGB;
      break;
  }

  csImage *img = new csImage(cinfo.output_width, cinfo.output_height, imageFormat);
  img->Copy(0, image_buffer);

  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);


  return img;
}

}