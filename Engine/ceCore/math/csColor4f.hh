#pragma once



#include <ceCore/csCoreExport.hh>
#include <ceCore/csDefs.hh>
#include <math.h>



namespace cryo {


struct CS_CORE_API csColor4f
{
public:
  float r;
  float g;
  float b;
  float a;

public:
  CS_FORCEINLINE csColor4f(const float* f)
    : r(f[0])
    , g(f[1])
    , b(f[2])
    , a(f[3])
  {

  }

  CS_FORCEINLINE csColor4f(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f)
    : r(r)
    , g(g)
    , b(b)
    , a(a)
  {

  }


  CS_FORCEINLINE bool operator==(const csColor4f &o) const
  {
    return CS_FLOAT_IS(r, o.r) && CS_FLOAT_IS(g, o.g) && CS_FLOAT_IS(b, o.b) && CS_FLOAT_IS(a, o.a);
  }

  CS_FORCEINLINE bool operator!=(const csColor4f& o) const
  {
    return !CS_FLOAT_IS(r, o.r) || !CS_FLOAT_IS(g, o.g) || !CS_FLOAT_IS(b, o.b) || !CS_FLOAT_IS(a, o.a);
  }

  CS_FORCEINLINE csColor4f operator*(float v) const
  {
    return csColor4f(r * v, g * v, b * v, a * v);
  }
};

}

