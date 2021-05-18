#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/defs.hh>
#include <spcCore/types.hh>
#include <spcCore/math/eaxis.hh>
#include <spcCore/math/vector3f.hh>


namespace spc
{


struct SPC_CORE_API Matrix3f
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
  SPC_FORCEINLINE Matrix3f(const Matrix3f &m) = default;

  SPC_FORCEINLINE explicit Matrix3f(float m00 = 1.0f, float m01 = 0.0f, float m02 = 0.0f, float m10 = 0.0f, float m11 = 1.0f,
                                    float m12 = 0.0f, float m20 = 0.0f, float m21 = 0.0f, float m22 = 1.0f)
      : m00(m00), m01(m01), m02(m02), m10(m10), m11(m11), m12(m12), m20(m20), m21(m21), m22(m22)
  {

  }

  SPC_FORCEINLINE explicit Matrix3f(const float *f)
      : m00(f[0]), m01(f[1]), m02(f[2]), m10(f[3]), m11(f[4]), m12(f[5]), m20(f[6]), m21(f[7]), m22(f[8])
  {
  }


  SPC_FORCEINLINE void SetXAxis(float x, float y, float z)
  {
    m00 = x;
    m01 = y;
    m02 = z;
  }

  SPC_FORCEINLINE void SetXAxis(const Vector3f &v)
  {
    m00 = v.x;
    m01 = v.y;
    m02 = v.z;
  }

  SPC_FORCEINLINE void SetYAxis(float x, float y, float z)
  {
    m10 = x;
    m11 = y;
    m12 = z;
  }

  SPC_FORCEINLINE void SetYAxis(const Vector3f &v)
  {
    m10 = v.x;
    m11 = v.y;
    m12 = v.z;
  }


  SPC_FORCEINLINE void SetZAxis(float x, float y, float z)
  {
    m20 = x;
    m21 = y;
    m22 = z;
  }

  SPC_FORCEINLINE void SetZAxis(const Vector3f &v)
  {
    m20 = v.x;
    m21 = v.y;
    m22 = v.z;
  }



  SPC_NODISCARD SPC_FORCEINLINE Vector3f GetXAxis() const
  {
    return Vector3f(m00, m01, m02);
  }

  SPC_NODISCARD SPC_FORCEINLINE Vector3f GetYAxis() const
  {
    return Vector3f(m10, m11, m12);
  }


  SPC_NODISCARD SPC_FORCEINLINE Vector3f GetZAxis() const
  {
    return Vector3f(m20, m21, m22);
  }

  SPC_NODISCARD SPC_FORCEINLINE Vector3f GetAxis(eAxis axis) const
  {
    switch (axis)
    {
      case eAxis::X: return Vector3f(m00, m01, m02);
      case eAxis::Y: return Vector3f(m10, m11, m12);
      case eAxis::Z: return Vector3f(m20, m21, m22);
    }
    return Vector3f(0.0f, 0.0f, 0.0f);
  }


  SPC_FORCEINLINE Matrix3f &operator=(const Matrix3f &m) = default;

  SPC_FORCEINLINE friend Matrix3f operator*(const Matrix3f &m0, const Matrix3f &m1)
  {
    float mm00 = m0.m00 * m1.m00 + m0.m10 * m1.m01 + m0.m20 * m1.m02;
    float mm01 = m0.m01 * m1.m00 + m0.m11 * m1.m01 + m0.m21 * m1.m02;
    float mm02 = m0.m02 * m1.m00 + m0.m12 * m1.m01 + m0.m22 * m1.m02;

    float mm10 = m0.m00 * m1.m10 + m0.m10 * m1.m11 + m0.m20 * m1.m12;
    float mm11 = m0.m01 * m1.m10 + m0.m11 * m1.m11 + m0.m21 * m1.m12;
    float mm12 = m0.m02 * m1.m10 + m0.m12 * m1.m11 + m0.m22 * m1.m12;

    float mm20 = m0.m00 * m1.m20 + m0.m10 * m1.m21 + m0.m20 * m1.m22;
    float mm21 = m0.m01 * m1.m20 + m0.m11 * m1.m21 + m0.m21 * m1.m22;
    float mm22 = m0.m02 * m1.m20 + m0.m12 * m1.m21 + m0.m22 * m1.m22;

    return Matrix3f(mm00, mm01, mm02,
                    mm10, mm11, mm12,
                    mm20, mm21, mm22);

  }


  SPC_FORCEINLINE friend Vector3f operator*(const Matrix3f &m, const Vector3f &v)
  {
    float x = m.m00 * v.x + m.m10 * v.y + m.m20 * v.z;
    float y = m.m01 * v.x + m.m11 * v.y + m.m21 * v.z;
    float z = m.m02 * v.x + m.m12 * v.y + m.m22 * v.z;
    return Vector3f(x, y, z);
  }

  void Debug(const std::string &msg = "") const
  {
    printf("spc::Matrix3f: %s\n", msg.c_str());
    printf("  %.2f %.2f %.2f\n", m00, m01, m02);
    printf("  %.2f %.2f %.2f\n", m10, m11, m12);
    printf("  %.2f %.2f %.2f\n", m20, m21, m22);
  }

};


}
