

#include <stdarg.h>
#include "jpegloader.hh"
#include "jpegloader.refl.hh"
#include "pngloader.hh"
#include "pngloader.refl.hh"


#include "jpegloader.refl.cc"
#include "pngloader.refl.cc"


#include <spcCore/classregistry.hh>

static void register_classes()
{
  spc::ClassRegistry *reg = spc::ClassRegistry::Get();
  // jpegloader.hh
  reg->Register(spc::img::JpegLoaderClass::Get());
  // pngloader.hh
  reg->Register(spc::img::PngLoaderClass::Get());
}

static void unregister_classes()
{
  spc::ClassRegistry *reg = spc::ClassRegistry::Get();
  // jpegloader.hh
  reg->Unregister(spc::img::JpegLoaderClass::Get());
  // pngloader.hh
  reg->Unregister(spc::img::PngLoaderClass::Get());
}

