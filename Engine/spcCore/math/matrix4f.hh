#pragma once

#include <cmath>
#include <spcCore/coreexport.hh>
#include <spcCore/defs.hh>
#include <spcCore/types.hh>
#include <spcCore/math/eaxis.hh>
#include <spcCore/math/matrix3f.hh>
#include <spcCore/math/vector2f.hh>
#include <spcCore/math/vector3f.hh>
#include <spcCore/math/vector4f.hh>


namespace spc
{


struct SPC_CORE_API Matrix4f
{
public:
  float m00;
  float m01;
  float m02;
  float m03;
  float m10;
  float m11;
  float m12;
  float m13;
  float m20;
  float m21;
  float m22;
  float m23;
  float m30;
  float m31;
  float m32;
  float m33;

public:

  SPC_FORCEINLINE explicit Matrix4f(float m00 = 1.0f, float m01 = 0.0f, float m02 = 0.0f, float m03 = 0.0f, float m10 = 0.0f, float m11 = 1.0f, float m12 = 0.0f, float m13 = 0.0f, float m20 = 0.0f, float m21 = 0.0f, float m22 = 1.0f, float m23 = 0.0f, float m30 = 0.0f, float m31 = 0.0f, float m32 = 0.0f, float m33 = 1.0f)
      : m00(m00)
        , m01(m01)
        , m02(m02)
        , m03(m03)
        , m10(m10)
        , m11(m11)
        , m12(m12)
        , m13(m13)
        , m20(m20)
        , m21(m21)
        , m22(m22)
        , m23(m23)
        , m30(m30)
        , m31(m31)
        , m32(m32)
        , m33(m33)
  {
  }

  SPC_FORCEINLINE explicit Matrix4f(const float *f)
      : m00(f[0])
        , m01(f[1])
        , m02(f[2])
        , m03(f[3])
        , m10(f[4])
        , m11(f[5])
        , m12(f[6])
        , m13(f[7])
        , m20(f[8])
        , m21(f[9])
        , m22(f[10])
        , m23(f[11])
        , m30(f[12])
        , m31(f[13])
        , m32(f[14])
        , m33(f[15])
  {
  }

  SPC_FORCEINLINE void SetIdentity()
  {
    m01 = m02 = m03 = m10 = m12 = m13 = m20 = m21 = m23 = m30 = m31 = m32 = 0.0f;
    m00 = m11 = m22 = m33 = 1.0f;
  }

  SPC_FORCEINLINE void SetXAxis(float x, float y, float z, float w = 0.0f)
  {
    m00 = x;
    m01 = y;
    m02 = z;
    m03 = w;
  }

  SPC_FORCEINLINE void SetXAxis(const Vector3f &v, float w = 0.0f)
  {
    m00 = v.x;
    m01 = v.y;
    m02 = v.z;
    m03 = w;
  }

  SPC_FORCEINLINE void SetXAxis(const Vector4f &v)
  {
    m00 = v.x;
    m01 = v.y;
    m02 = v.z;
    m03 = v.w;
  }

  SPC_FORCEINLINE void SetYAxis(float x, float y, float z, float w = 0.0f)
  {
    m10 = x;
    m11 = y;
    m12 = z;
    m13 = w;
  }

  SPC_FORCEINLINE void SetYAxis(const Vector3f &v, float w = 0.0f)
  {
    m10 = v.x;
    m11 = v.y;
    m12 = v.z;
    m13 = w;
  }


  SPC_FORCEINLINE void SetYAxis(const Vector4f &v)
  {
    m10 = v.x;
    m11 = v.y;
    m12 = v.z;
    m13 = v.w;
  }


  SPC_FORCEINLINE void SetZAxis(float x, float y, float z, float w = 0.0f)
  {
    m20 = x;
    m21 = y;
    m22 = z;
    m23 = w;
  }

  SPC_FORCEINLINE void SetZAxis(const Vector3f &v, float w = 0.0f)
  {
    m20 = v.x;
    m21 = v.y;
    m22 = v.z;
    m23 = w;
  }

  SPC_FORCEINLINE void SetZAxis(const Vector4f &v)
  {
    m20 = v.x;
    m21 = v.y;
    m22 = v.z;
    m23 = v.w;
  }


  SPC_FORCEINLINE void SetTranslation(float x, float y, float z, float w = 1.0f)
  {
    m30 = x;
    m31 = y;
    m32 = z;
    m33 = w;
  }

  SPC_FORCEINLINE void SetTranslation(const Vector3f &tr, float w = 1.0f)
  {
    m30 = tr.x;
    m31 = tr.y;
    m32 = tr.z;
    m33 = w;
  }

  SPC_FORCEINLINE void SetTranslation(const Vector4f &tr)
  {
    m30 = tr.x;
    m31 = tr.y;
    m32 = tr.z;
    m33 = tr.w;
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
      case eAxis::X:
        return Vector3f(m00, m01, m02);
      case eAxis::Y:
        return Vector3f(m10, m11, m12);
      case eAxis::Z:
        return Vector3f(m20, m21, m22);
    }
    return Vector3f(0.0f, 0.0f, 0.0f);
  }


  SPC_NODISCARD SPC_FORCEINLINE Vector3f GetTranslation() const
  {
    return Vector3f(m30, m31, m32);
  }


  SPC_FORCEINLINE void ClearRotation()
  {
    m00 = m11 = m22 = 1.0f;
    m01 = m02 = m10 = m12 = m20 = m21 = 0.0f;
  }


  SPC_FORCEINLINE void SetRotationX(float angle)
  {
    float c = std::cos(angle);
    float s = std::sin(angle);
    m11 = c;
    m12 = s;
    m21 = -s;
    m22 = c;
  }

  SPC_FORCEINLINE void SetRotationY(float angle)
  {
    float c = std::cos(angle);
    float s = std::sin(angle);
    m00 = c;
    m02 = -s;
    m20 = s;
    m22 = c;
  }

  SPC_FORCEINLINE void SetRotationZ(float angle)
  {
    float c = std::cos(angle);
    float s = std::sin(angle);
    m00 = c;
    m01 = s;
    m10 = -s;
    m11 = c;
  }

  SPC_FORCEINLINE void SetScale(Vector3f s)
  {
    m00 = s.x;
    m11 = s.y;
    m22 = s.z;
  }

  SPC_FORCEINLINE void Scale(Vector3f s)
  {
    m00 *= s.x;
    m11 *= s.y;
    m22 *= s.z;
  }

  SPC_FORCEINLINE Matrix4f &SetRotation(const Vector3f &axis, float angle)
  {
    float c = std::cos(angle);
    float s = std::sin(angle);

    float x = axis.x;
    float y = axis.y;
    float z = axis.z;

    float ic = 1.0f - c;
    m00 = x * x * ic + c;
    m10 = x * y * ic - z * s;
    m20 = x * z * ic + y * s;
    m01 = y * x * ic + z * s;
    m11 = y * y * ic + c;
    m21 = y * z * ic - x * s;
    m02 = z * x * ic - y * s;
    m12 = z * y * ic + x * s;
    m22 = z * z * ic + c;

    return *this;
  }

  SPC_NODISCARD SPC_FORCEINLINE Matrix4f Inverted() const
  {
    float v0 = m20 * m31 - m21 * m30;
    float v1 = m20 * m32 - m22 * m30;
    float v2 = m20 * m33 - m23 * m30;
    float v3 = m21 * m32 - m22 * m31;
    float v4 = m21 * m33 - m23 * m31;
    float v5 = m22 * m33 - m23 * m32;

    float t00 = +(v5 * m11 - v4 * m12 + v3 * m13);
    float t10 = -(v5 * m10 - v2 * m12 + v1 * m13);
    float t20 = +(v4 * m10 - v2 * m11 + v0 * m13);
    float t30 = -(v3 * m10 - v1 * m11 + v0 * m12);

    float invDet = 1.0f / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

    float lm00 = t00 * invDet;
    float lm10 = t10 * invDet;
    float lm20 = t20 * invDet;
    float lm30 = t30 * invDet;

    float lm01 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
    float lm11 = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
    float lm21 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
    float lm31 = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

    v0 = m10 * m31 - m11 * m30;
    v1 = m10 * m32 - m12 * m30;
    v2 = m10 * m33 - m13 * m30;
    v3 = m11 * m32 - m12 * m31;
    v4 = m11 * m33 - m13 * m31;
    v5 = m12 * m33 - m13 * m32;

    float lm02 = +(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
    float lm12 = -(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
    float lm22 = +(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
    float lm32 = -(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

    v0 = m21 * m10 - m20 * m11;
    v1 = m22 * m10 - m20 * m12;
    v2 = m23 * m10 - m20 * m13;
    v3 = m22 * m11 - m21 * m12;
    v4 = m23 * m11 - m21 * m13;
    v5 = m23 * m12 - m22 * m13;

    float lm03 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
    float lm13 = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
    float lm23 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
    float lm33 = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

    return Matrix4f(lm00, lm01, lm02, lm03,
                    lm10, lm11, lm12, lm13,
                    lm20, lm21, lm22, lm23,
                    lm30, lm31, lm32, lm33);
  }

  SPC_FORCEINLINE void Invert()
  {
    float v0 = m20 * m31 - m21 * m30;
    float v1 = m20 * m32 - m22 * m30;
    float v2 = m20 * m33 - m23 * m30;
    float v3 = m21 * m32 - m22 * m31;
    float v4 = m21 * m33 - m23 * m31;
    float v5 = m22 * m33 - m23 * m32;

    float t00 = +(v5 * m11 - v4 * m12 + v3 * m13);
    float t10 = -(v5 * m10 - v2 * m12 + v1 * m13);
    float t20 = +(v4 * m10 - v2 * m11 + v0 * m13);
    float t30 = -(v3 * m10 - v1 * m11 + v0 * m12);

    float invDet = 1.0f / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

    float lm00 = t00 * invDet;
    float lm10 = t10 * invDet;
    float lm20 = t20 * invDet;
    float lm30 = t30 * invDet;

    float lm01 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
    float lm11 = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
    float lm21 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
    float lm31 = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

    v0 = m10 * m31 - m11 * m30;
    v1 = m10 * m32 - m12 * m30;
    v2 = m10 * m33 - m13 * m30;
    v3 = m11 * m32 - m12 * m31;
    v4 = m11 * m33 - m13 * m31;
    v5 = m12 * m33 - m13 * m32;

    float lm02 = +(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
    float lm12 = -(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
    float lm22 = +(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
    float lm32 = -(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

    v0 = m21 * m10 - m20 * m11;
    v1 = m22 * m10 - m20 * m12;
    v2 = m23 * m10 - m20 * m13;
    v3 = m22 * m11 - m21 * m12;
    v4 = m23 * m11 - m21 * m13;
    v5 = m23 * m12 - m22 * m13;

    float lm03 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
    float lm13 = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
    float lm23 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
    float lm33 = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

    m00 = lm00;
    m01 = lm01;
    m02 = lm02;
    m03 = lm03;

    m10 = lm10;
    m11 = lm11;
    m12 = lm12;
    m13 = lm13;

    m20 = lm20;
    m21 = lm21;
    m22 = lm22;
    m23 = lm23;

    m30 = lm30;
    m31 = lm31;
    m32 = lm32;
    m33 = lm33;
  }

  SPC_NODISCARD SPC_FORCEINLINE Matrix4f Transposed() const
  {
    float mm00 = m00;
    float mm01 = m10;
    float mm02 = m20;
    float mm03 = m30;
    float mm10 = m01;
    float mm11 = m11;
    float mm12 = m21;
    float mm13 = m31;
    float mm20 = m02;
    float mm21 = m12;
    float mm22 = m22;
    float mm23 = m32;
    float mm30 = m03;
    float mm31 = m13;
    float mm32 = m23;
    float mm33 = m33;
    return Matrix4f(
        mm00, mm01, mm02, mm03,
        mm10, mm11, mm12, mm13,
        mm20, mm21, mm22, mm23,
        mm30, mm31, mm32, mm33
    );
  }

#define SWAP(a, b, s) (s) = (a); (a)=(b); (b)=(s)

  SPC_FORCEINLINE void Transpose()
  {
    float s;
    SWAP(m01, m10, s);
    SWAP(m02, m20, s);
    SWAP(m03, m30, s);
    SWAP(m12, m21, s);
    SWAP(m13, m31, s);
    SWAP(m23, m32, s);
  }

#undef SWAP

  SPC_FORCEINLINE void SetLookAt(const Vector3f &eye, const Vector3f &spot, const Vector3f &up)
  {

    Vector3f zAxis = (spot - eye).Normalized();
    Vector3f xAxis = (up % zAxis).Normalized();
    Vector3f yAxis = zAxis % xAxis;
//    Vector3f::Sub(spot, eye, zAxis).Normalize();
//    Vector3f::Cross(up, zAxis, xAxis).Normalize();
//    Vector3f::Cross(zAxis, xAxis, yAxis);
    SetXAxis(xAxis.x, yAxis.x, zAxis.x);
    SetYAxis(xAxis.y, yAxis.y, zAxis.y);
    SetZAxis(xAxis.z, yAxis.z, zAxis.z);
    SetTranslation(-xAxis.Dot(eye), -yAxis.Dot(eye), -zAxis.Dot(eye));
  }

  SPC_FORCEINLINE void SetLookAtInv(const Vector3f &eye, const Vector3f &spot, const Vector3f &up)
  {
//    Vector3f xAxis, yAxis, zAxis;
//    Vector3f::Sub(eye, spot, zAxis).Normalize();
//    Vector3f::Cross(up, zAxis, xAxis).Normalize();
//    Vector3f::Cross(zAxis, xAxis, yAxis);
//
//    Vector3f::Sub(spot, eye, zAxis).Normalize();
//    Vector3f::Cross(up, zAxis, xAxis).Normalize();
//    Vector3f::Cross(zAxis, xAxis, yAxis);

    Vector3f zAxis = (spot - eye).Normalized();
    Vector3f xAxis = (up % zAxis).Normalized();
    Vector3f yAxis = zAxis % xAxis;

    SetXAxis(xAxis);
    SetYAxis(yAxis);
    SetZAxis(zAxis);
    SetTranslation(eye);
  }

  SPC_FORCEINLINE Matrix4f &operator=(const Matrix4f &other) = default;

  SPC_FORCEINLINE friend Matrix4f operator*(const Matrix4f &m0, const Matrix4f &m1)
  {
    float mm00 = m0.m00 * m1.m00 + m0.m10 * m1.m01 + m0.m20 * m1.m02 + m0.m30 * m1.m03;
    float mm01 = m0.m01 * m1.m00 + m0.m11 * m1.m01 + m0.m21 * m1.m02 + m0.m31 * m1.m03;
    float mm02 = m0.m02 * m1.m00 + m0.m12 * m1.m01 + m0.m22 * m1.m02 + m0.m32 * m1.m03;
    float mm03 = m0.m03 * m1.m00 + m0.m13 * m1.m01 + m0.m23 * m1.m02 + m0.m33 * m1.m03;

    float mm10 = m0.m00 * m1.m10 + m0.m10 * m1.m11 + m0.m20 * m1.m12 + m0.m30 * m1.m13;
    float mm11 = m0.m01 * m1.m10 + m0.m11 * m1.m11 + m0.m21 * m1.m12 + m0.m31 * m1.m13;
    float mm12 = m0.m02 * m1.m10 + m0.m12 * m1.m11 + m0.m22 * m1.m12 + m0.m32 * m1.m13;
    float mm13 = m0.m03 * m1.m10 + m0.m13 * m1.m11 + m0.m23 * m1.m12 + m0.m33 * m1.m13;

    float mm20 = m0.m00 * m1.m20 + m0.m10 * m1.m21 + m0.m20 * m1.m22 + m0.m30 * m1.m23;
    float mm21 = m0.m01 * m1.m20 + m0.m11 * m1.m21 + m0.m21 * m1.m22 + m0.m31 * m1.m23;
    float mm22 = m0.m02 * m1.m20 + m0.m12 * m1.m21 + m0.m22 * m1.m22 + m0.m32 * m1.m23;
    float mm23 = m0.m03 * m1.m20 + m0.m13 * m1.m21 + m0.m23 * m1.m22 + m0.m33 * m1.m23;

    float mm30 = m0.m00 * m1.m30 + m0.m10 * m1.m31 + m0.m20 * m1.m32 + m0.m30 * m1.m33;
    float mm31 = m0.m01 * m1.m30 + m0.m11 * m1.m31 + m0.m21 * m1.m32 + m0.m31 * m1.m33;
    float mm32 = m0.m02 * m1.m30 + m0.m12 * m1.m31 + m0.m22 * m1.m32 + m0.m32 * m1.m33;
    float mm33 = m0.m03 * m1.m30 + m0.m13 * m1.m31 + m0.m23 * m1.m32 + m0.m33 * m1.m33;

    return Matrix4f(
        mm00, mm01, mm02, mm03,
        mm10, mm11, mm12, mm13,
        mm20, mm21, mm22, mm23,
        mm30, mm31, mm32, mm33
    );
  }


  SPC_FORCEINLINE friend Vector3f operator*(const Matrix4f &m, const Vector3f &v)
  {
    float x = m.m00 * v.x + m.m10 * v.y + m.m20 * v.z;
    float y = m.m01 * v.x + m.m11 * v.y + m.m21 * v.z;
    float z = m.m02 * v.x + m.m12 * v.y + m.m22 * v.z;
    return Vector3f(x, y, z);
  }


  SPC_NODISCARD SPC_FORCEINLINE static Vector3f Mult(const Matrix4f &m, const Vector3f &v)
  {
    float x = m.m00 * v.x + m.m10 * v.y + m.m20 * v.z;
    float y = m.m01 * v.x + m.m11 * v.y + m.m21 * v.z;
    float z = m.m02 * v.x + m.m12 * v.y + m.m22 * v.z;
    return Vector3f(x, y, z);
  }



  SPC_FORCEINLINE friend Vector4f operator*(const Matrix4f &m, const Vector4f &v)
  {
    float x = m.m00 * v.x + m.m10 * v.y + m.m20 * v.z + m.m30 * v.w;
    float y = m.m01 * v.x + m.m11 * v.y + m.m21 * v.z + m.m31 * v.w;
    float z = m.m02 * v.x + m.m12 * v.y + m.m22 * v.z + m.m32 * v.w;
    float w = m.m03 * v.x + m.m13 * v.y + m.m23 * v.z + m.m33 * v.w;
    return Vector4f(x, y, z, w);
  }



  SPC_NODISCARD SPC_FORCEINLINE static Vector4f Mult(const Matrix4f &m, const Vector4f &v)
  {
    float x = m.m00 * v.x + m.m10 * v.y + m.m20 * v.z + m.m30 * v.w;
    float y = m.m01 * v.x + m.m11 * v.y + m.m21 * v.z + m.m31 * v.w;
    float z = m.m02 * v.x + m.m12 * v.y + m.m22 * v.z + m.m32 * v.w;
    float w = m.m03 * v.x + m.m13 * v.y + m.m23 * v.z + m.m33 * v.w;
    return Vector4f(x, y, z, w);
  }



  SPC_NODISCARD SPC_FORCEINLINE static Vector3f Transform(const Matrix4f &m, const Vector3f &v)
  {
    float x = m.m00 * v.x + m.m10 * v.y + m.m20 * v.z + m.m30;
    float y = m.m01 * v.x + m.m11 * v.y + m.m21 * v.z + m.m31;
    float z = m.m02 * v.x + m.m12 * v.y + m.m22 * v.z + m.m32;
    return Vector3f(x, y, z);
  }

  explicit SPC_FORCEINLINE operator Matrix3f() const
  {
    return Matrix3f(
        m00, m01, m02,
        m10, m11, m12,
        m20, m21, m22
        );
  }

  SPC_FORCEINLINE void Debug(const char *message = nullptr) const
  {
    if (message)
    {
      printf("spc::Matrix4f: %s\n", message);
    }
    else
    {
      printf("spc::Matrix4f:\n");
    }

    printf("  %.2f %.2f %.2f %.2f\n", m00, m01, m02, m03);
    printf("  %.2f %.2f %.2f %.2f\n", m10, m11, m12, m13);
    printf("  %.2f %.2f %.2f %.2f\n", m20, m21, m22, m23);
    printf("  %.2f %.2f %.2f %.2f\n", m30, m31, m32, m33);
    fflush(stdout);
  }


  SPC_NODISCARD SPC_FORCEINLINE bool IsIdentity() const
  {
    return SPC_FLOAT_IS_ONE(m00) &&
           SPC_FLOAT_IS_ONE(m11) &&
           SPC_FLOAT_IS_ONE(m22) &&
           SPC_FLOAT_IS_ONE(m33) &&
           SPC_FLOAT_IS_ZERO(m01) &&
           SPC_FLOAT_IS_ZERO(m02) &&
           SPC_FLOAT_IS_ZERO(m03) &&
           SPC_FLOAT_IS_ZERO(m10) &&
           SPC_FLOAT_IS_ZERO(m12) &&
           SPC_FLOAT_IS_ZERO(m13) &&
           SPC_FLOAT_IS_ZERO(m20) &&
           SPC_FLOAT_IS_ZERO(m21) &&
           SPC_FLOAT_IS_ZERO(m23) &&
           SPC_FLOAT_IS_ZERO(m30) &&
           SPC_FLOAT_IS_ZERO(m31) &&
           SPC_FLOAT_IS_ZERO(m32);
  }

};


}
