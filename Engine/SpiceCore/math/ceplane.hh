#pragma once

#include <cecore/ceexport.hh>
#include <cecore/math/cevector3f.hh>


struct CEC_API cePlane
{
public:
  float x;
  float y;
  float z;
  float d;

public:
  CE_FORCEINLINE cePlane(float x = 0.0f, float y = 0.0f, float z = 0.0f, float d = 0.0f)
    : x(x)
    , y(y)
    , z(z)
    , d(d)
  {

  }

  CE_FORCEINLINE cePlane(const ceVector3f& pos, const ceVector3f& norm)
  {
    ceVector3f nn;
    norm.Normalized(nn);
    x = nn.x;
    y = nn.y;
    z = nn.z;
    d = -pos.Dot(nn);
  }

  CE_FORCEINLINE float Distance(const ceVector3f& pos) const
  {
    return x * pos.x + y * pos.y + z * pos.z + d;
  }


  CE_FORCEINLINE ceVector3f& ReflectPoint(const ceVector3f& p, ceVector3f& r) const
  {
    ceVector3f origin, diff;
    ceVector3f normal = ceVector3f(x, y, z);
    ceVector3f::Mul(normal, -d, origin);
    ceVector3f::Sub(p, origin, diff);

    ceVector3f::Mul(normal, 2.0f * diff.Dot(normal), r);
    ceVector3f::Sub(diff, r, r);
    return ceVector3f::Add(r, origin, r);
  }

  ceVector3f& ReflectDirection(const ceVector3f& d, ceVector3f& r) const
  {
    ceVector3f normal = ceVector3f(x, y, z);
    ceVector3f::Mul(normal, 2.0f * d.Dot(normal), r);
    return ceVector3f::Sub(d, r, r);
  }


};


