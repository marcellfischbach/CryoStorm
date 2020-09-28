#pragma once

#include <SpiceCore/coreexport.hh>
#include <SpiceCore/defs.hh>
#include <SpiceCore/types.hh>
#include <SpiceCore/math/vector3f.hh>



namespace Spice {


struct SPICE_CORE_API Matrix3f
{
public:
  float m00;
  float m01;
  float m02;
  float m10;
  float m11;
  float m12;
  float m20;
  float m21;
  float m22;

public:
  SPICE_FORCEINLINE Matrix3f(const float* f)
    : m00(f[0])
    , m01(f[1])
    , m02(f[2])
    , m10(f[3])
    , m11(f[4])
    , m12(f[5])
    , m20(f[6])
    , m21(f[7])
    , m22(f[8])
  {
  }

  SPICE_FORCEINLINE Matrix3f(float m00 = 1.0f, float m01 = 0.0f, float m02 = 0.0f, float m10 = 0.0f, float m11 = 1.0f, float m12 = 0.0f, float m20 = 0.0f, float m21 = 0.0f, float m22 = 1.0f)
    : m00(m00)
    , m01(m01)
    , m02(m02)
    , m10(m10)
    , m11(m11)
    , m12(m12)
    , m20(m20)
    , m21(m21)
    , m22(m22)
  {

  }

  SPICE_FORCEINLINE void SetXAxis(float x, float y, float z)
  {
    m00 = x;
    m01 = y;
    m02 = z;
  }

  SPICE_FORCEINLINE void SetXAxis(const Vector3f& v)
  {
    m00 = v.x;
    m01 = v.y;
    m02 = v.z;
  }

  SPICE_FORCEINLINE void SetYAxis(float x, float y, float z)
  {
    m10 = x;
    m11 = y;
    m12 = z;
  }

  SPICE_FORCEINLINE void SetYAxis(const Vector3f& v)
  {
    m10 = v.x;
    m11 = v.y;
    m12 = v.z;
  }


  SPICE_FORCEINLINE void SetZAxis(float x, float y, float z)
  {
    m20 = x;
    m21 = y;
    m22 = z;
  }

  SPICE_FORCEINLINE void SetZAxis(const Vector3f& v)
  {
    m20 = v.x;
    m21 = v.y;
    m22 = v.z;
  }

  SPICE_FORCEINLINE static ceMatrix3f& Mult(const Matrix3f& m0, const Matrix3f& m1, ceMatrix3f& r)
  {
    float m00 = m0.m00 * m1.m00 + m0.m10 * m1.m01 + m0.m20 * m1.m02;
    float m01 = m0.m01 * m1.m00 + m0.m11 * m1.m01 + m0.m21 * m1.m02;
    float m02 = m0.m02 * m1.m00 + m0.m12 * m1.m01 + m0.m22 * m1.m02;

    float m10 = m0.m00 * m1.m10 + m0.m10 * m1.m11 + m0.m20 * m1.m12;
    float m11 = m0.m01 * m1.m10 + m0.m11 * m1.m11 + m0.m21 * m1.m12;
    float m12 = m0.m02 * m1.m10 + m0.m12 * m1.m11 + m0.m22 * m1.m12;

    float m20 = m0.m00 * m1.m20 + m0.m10 * m1.m21 + m0.m20 * m1.m22;
    float m21 = m0.m01 * m1.m20 + m0.m11 * m1.m21 + m0.m21 * m1.m22;
    float m22 = m0.m02 * m1.m20 + m0.m12 * m1.m21 + m0.m22 * m1.m22;


    r.m00 = m00;
    r.m01 = m01;
    r.m02 = m02;
    r.m10 = m10;
    r.m11 = m11;
    r.m12 = m12;
    r.m20 = m20;
    r.m21 = m21;
    r.m22 = m22;
    return r;
  }


  static Vector3f& Mult(const Matrix3f& m, const Vector3f& v, Vector3f& res)
  {
    float x = m.m00 * v.x + m.m10 * v.y + m.m20 * v.z;
    float y = m.m01 * v.x + m.m11 * v.y + m.m21 * v.z;
    float z = m.m02 * v.x + m.m12 * v.y + m.m22 * v.z;
    res.x = x;
    res.y = y;
    res.z = z;
    return res;
  }


};



}
