#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/math/vector3f.hh>


namespace cryo
{


struct CS_CORE_API Plane
{
public:
  float x;
  float y;
  float z;
  float d;

public:
  CS_FORCEINLINE explicit Plane(float x = 0.0f, float y = 0.0f, float z = 0.0f, float d = 0.0f)
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

  CS_FORCEINLINE Plane(const Vector3f& pos, const Vector3f& norm)
  {
    Vector3f nn = norm.Normalized();
    x = nn.x;
    y = nn.y;
    z = nn.z;
    d = -pos.Dot(nn);
  }

  CS_FORCEINLINE void Set(const Vector3f& pos, const Vector3f& norm)
  {
    Vector3f nn = norm.Normalized();
    x = nn.x;
    y = nn.y;
    z = nn.z;
    d = -pos.Dot(nn);
  }

  CS_FORCEINLINE Plane(const Vector3f& p0, const Vector3f &p1, const Vector3f &p2)
  {
    Vector3f n0 = (p1 - p0).Normalized();
    Vector3f n1 = (p2 - p0).Normalized();
    Vector3f norm = n0 % n1;


    Vector3f nn = norm.Normalized();
    x = nn.x;
    y = nn.y;
    z = nn.z;
    d = -p0.Dot(nn);
  }

  CS_FORCEINLINE void Set(const Vector3f& p0, const Vector3f &p1, const Vector3f &p2)
  {
    Vector3f n0 = (p1 - p0).Normalized();
    Vector3f n1 = (p2 - p0).Normalized();
    Vector3f norm = n0 % n1;


    Vector3f nn = norm.Normalized();
    x = nn.x;
    y = nn.y;
    z = nn.z;
    d = -p0.Dot(nn);
  }


  CS_NODISCARD CS_FORCEINLINE float Distance(const Vector3f& pos) const
  {
    return x * pos.x + y * pos.y + z * pos.z + d;
  }


  CS_NODISCARD CS_FORCEINLINE Vector3f ReflectPoint(const Vector3f& p) const
  {
    Vector3f normal = Vector3f(x, y, z);
    Vector3f origin = normal * -d;
    Vector3f diff = p - origin;

    return diff - normal * (2.0f * (diff * normal)) + origin;

//    Vector3f::Mul(normal, -d, origin);
//    Vector3f::Sub(p, origin, diff);
//
//    Vector3f::Mul(normal, 2.0f * diff.Dot(normal), r);
//    Vector3f::Sub(diff, r, r);
//    return Vector3f::Add(r, origin, r);
  }

  CS_NODISCARD CS_FORCEINLINE Vector3f ReflectDirection(const Vector3f& d) const
  {
    Vector3f normal = Vector3f(x, y, z);
    return d - normal * (2.0f * (d * normal));

//    Vector3f::Mul(normal, 2.0f * d.Dot(normal), r);
//    return Vector3f::Sub(d, r, r);
  }


};


}

