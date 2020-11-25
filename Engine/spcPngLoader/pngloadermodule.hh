
#pragma once

#include <spcPngLoader/pngloaderexport.hh>

namespace spc
{

class SPC_PNGLOADER_API PngLoaderModule
{
public:
  static bool Register(int argc, char** argv);
  static bool Initialize(int argc, char** argv);

private:
  PngLoaderModule() { }
};

}