#pragma once



#include <ceCore/coreexport.hh>
#include <ceCore/defs.hh>
#include <math.h>



namespace ce {


struct CE_CORE_API Color4f
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


  CE_FORCEINLINE bool operator== (const Color4f &o) const
  {
    return CE_FLOAT_IS(r, o.r) && CE_FLOAT_IS(g, o.g) && CE_FLOAT_IS(b, o.b) && CE_FLOAT_IS(a, o.a);
  }

  CE_FORCEINLINE bool operator!= (const Color4f& o) const
  {
    return !CE_FLOAT_IS(r, o.r) || !CE_FLOAT_IS(g, o.g) || !CE_FLOAT_IS(b, o.b) || !CE_FLOAT_IS(a, o.a);
  }

  CE_FORCEINLINE Color4f operator* (float v) const
  {
    return Color4f(r*v, g*v, b*v, a*v);
  }
};

}

