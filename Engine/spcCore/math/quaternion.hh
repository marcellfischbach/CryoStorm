
#pragma  once

#include <spcCore/coreexport.hh>
#include <spcCore/defs.hh>
#include <math.h>

namespace spc
{

struct SPC_CORE_API Quaternion
{
  float x;
  float y;
  float z;
  float w;
  
  Quaternion(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f)
    : x(x)
    , y(y)
    , z(z)
    , w(w)
  {
  
  }
  
  SPC_FORCEINLINE Quaternion &Normalize()
  {
    float l = sqrt(x*x + y*y + z*z + w*w);
    x /= l;
    y /= l;
    z /= l;
    w /= l;
    return *this;
  }
  
};
}