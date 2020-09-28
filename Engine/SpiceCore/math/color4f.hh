#pragma once



#include <SpiceCore/coreexport.hh>
#include <SpiceCore/defs.hh>
#include <math.h>



namespace Spice {


struct SPICE_CORE_API Color4f
{
public:
  float r;
  float g;
  float b;
  float a;

public:
  CE_FORCEINLINE Color4f(const float* f)
    : r(f[0])
    , g(f[1])
    , b(f[2])
    , a(f[3])
  {

  }

  CE_FORCEINLINE Color4f(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f)
    : r(r)
    , g(g)
    , b(b)
    , a(a)
  {

  }

};

}

