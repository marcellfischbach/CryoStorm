#pragma once

#include <ceCore/csCoreExport.hh>
#include <ceCore/math/csVector3f.hh>


namespace cryo
{


struct CS_CORE_API csPlane
{
public:
  float x;
  float y;
  float z;
  float d;

public:
  CS_FORCEINLINE explicit csPlane(float x = 0.0f, float y = 0.0f, float z = 0.0f, float d = 0.0f)
          : x(x)
          , y(y)
          , z(z)
          , d(d)
  {

  }

  CS_FORCEINLINE void Set(float x = 0.0f, float y = 0.0f, float z = 0.0f, float d = 0.0f)
  {
    this->x = x;
    this->y = y;
    this->z = z;
    this->d = d;
  }

  CS_FORCEINLINE csPlane(const csVector3f& pos, const csVector3f& norm)
  {
    csVector3f nn = norm.Normalized();
    x = nn.x;
    y = nn.y;
    z = nn.z;
    d = -pos.Dot(nn);
  }

  CS_FORCEINLINE void Set(const csVector3f& pos, const csVector3f& norm)
  {
    csVector3f nn = norm.Normalized();
    x = nn.x;
    y = nn.y;
    z = nn.z;
    d = -pos.Dot(nn);
  }

  CS_FORCEINLINE csPlane(const csVector3f& p0, const csVector3f &p1, const csVector3f &p2)
  {
    csVector3f n0   = (p1 - p0).Normalized();
    csVector3f n1   = (p2 - p0).Normalized();
    csVector3f norm = n0 % n1;


    csVector3f nn = norm.Normalized();
    x = nn.x;
    y = nn.y;
    z = nn.z;
    d = -p0.Dot(nn);
  }

  CS_FORCEINLINE void Set(const csVector3f& p0, const csVector3f &p1, const csVector3f &p2)
  {
    csVector3f n0   = (p1 - p0).Normalized();
    csVector3f n1   = (p2 - p0).Normalized();
    csVector3f norm = n0 % n1;


    csVector3f nn = norm.Normalized();
    x = nn.x;
    y = nn.y;
    z = nn.z;
    d = -p0.Dot(nn);
  }


  CS_NODISCARD CS_FORCEINLINE float Distance(const csVector3f& pos) const
  {
    return x * pos.x + y * pos.y + z * pos.z + d;
  }


  CS_NODISCARD CS_FORCEINLINE csVector3f ReflectPoint(const csVector3f& p) const
  {
    csVector3f normal = csVector3f(x, y, z);
    csVector3f origin = normal * -d;
    csVector3f diff   = p - origin;

    return diff - normal * (2.0f * (diff * normal)) + origin;

//    Vector3f::Mul(normal, -d, origin);
//    Vector3f::Sub(p, origin, diff);
//
//    Vector3f::Mul(normal, 2.0f * diff.Dot(normal), r);
//    Vector3f::Sub(diff, r, r);
//    return Vector3f::Add(r, origin, r);
  }

  CS_NODISCARD CS_FORCEINLINE csVector3f ReflectDirection(const csVector3f& d) const
  {
    csVector3f normal = csVector3f(x, y, z);
    return d - normal * (2.0f * (d * normal));

//    Vector3f::Mul(normal, 2.0f * d.Dot(normal), r);
//    return Vector3f::Sub(d, r, r);
  }


};


}

