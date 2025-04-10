#pragma once

#include <cmath>
#include <csCore/csCoreExport.hh>
#include <csCore/csDefs.hh>
#include <csCore/csTypes.hh>
#include <csCore/math/eAxis.hh>
#include <csCore/math/csMatrix3f.hh>
#include <csCore/math/csVector2f.hh>
#include <csCore/math/csVector3f.hh>
#include <csCore/math/csVector4f.hh>


namespace cs
{


struct CS_CORE_API csMatrix4f
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

  CS_FORCEINLINE explicit csMatrix4f(float m00 = 1.0f,
                                     float m01 = 0.0f,
                                     float m02 = 0.0f,
                                     float m03 = 0.0f,
                                     float m10 = 0.0f,
                                     float m11 = 1.0f,
                                     float m12 = 0.0f,
                                     float m13 = 0.0f,
                                     float m20 = 0.0f,
                                     float m21 = 0.0f,
                                     float m22 = 1.0f,
                                     float m23 = 0.0f,
                                     float m30 = 0.0f,
                                     float m31 = 0.0f,
                                     float m32 = 0.0f,
                                     float m33 = 1.0f)
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

  CS_FORCEINLINE explicit csMatrix4f(const float *f)
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

  CS_FORCEINLINE void Set(float m00,
                          float m01,
                          float m02,
                          float m03,
                          float m10,
                          float m11,
                          float m12,
                          float m13,
                          float m20,
                          float m21,
                          float m22,
                          float m23,
                          float m30,
                          float m31,
                          float m32,
                          float m33)
  {
    this->m00 = m00;
    this->m01 = m01;
    this->m02 = m02;
    this->m03 = m03;

    this->m10 = m10;
    this->m11 = m11;
    this->m12 = m12;
    this->m13 = m13;

    this->m20 = m20;
    this->m21 = m21;
    this->m22 = m22;
    this->m23 = m23;

    this->m30 = m30;
    this->m31 = m31;
    this->m32 = m32;
    this->m33 = m33;
  }

  CS_FORCEINLINE void SetIdentity()
  {
    m01 = m02 = m03 = m10 = m12 = m13 = m20 = m21 = m23 = m30 = m31 = m32 = 0.0f;
    m00 = m11 = m22 = m33 = 1.0f;
  }

  CS_FORCEINLINE void SetXAxis(float x, float y, float z, float w = 0.0f)
  {
    m00 = x;
    m01 = y;
    m02 = z;
    m03 = w;
  }

  CS_FORCEINLINE void SetXAxis(const csVector3f &v, float w = 0.0f)
  {
    m00 = v.x;
    m01 = v.y;
    m02 = v.z;
    m03 = w;
  }

  CS_FORCEINLINE void SetXAxis(const csVector4f &v)
  {
    m00 = v.x;
    m01 = v.y;
    m02 = v.z;
    m03 = v.w;
  }

  CS_FORCEINLINE void SetYAxis(float x, float y, float z, float w = 0.0f)
  {
    m10 = x;
    m11 = y;
    m12 = z;
    m13 = w;
  }

  CS_FORCEINLINE void SetYAxis(const csVector3f &v, float w = 0.0f)
  {
    m10 = v.x;
    m11 = v.y;
    m12 = v.z;
    m13 = w;
  }


  CS_FORCEINLINE void SetYAxis(const csVector4f &v)
  {
    m10 = v.x;
    m11 = v.y;
    m12 = v.z;
    m13 = v.w;
  }


  CS_FORCEINLINE void SetZAxis(float x, float y, float z, float w = 0.0f)
  {
    m20 = x;
    m21 = y;
    m22 = z;
    m23 = w;
  }

  CS_FORCEINLINE void SetZAxis(const csVector3f &v, float w = 0.0f)
  {
    m20 = v.x;
    m21 = v.y;
    m22 = v.z;
    m23 = w;
  }

  CS_FORCEINLINE void SetZAxis(const csVector4f &v)
  {
    m20 = v.x;
    m21 = v.y;
    m22 = v.z;
    m23 = v.w;
  }


  CS_FORCEINLINE void SetTranslation(float x, float y, float z, float w = 1.0f)
  {
    m30 = x;
    m31 = y;
    m32 = z;
    m33 = w;
  }

  CS_FORCEINLINE void SetTranslation(const csVector3f &tr, float w = 1.0f)
  {
    m30 = tr.x;
    m31 = tr.y;
    m32 = tr.z;
    m33 = w;
  }

  CS_FORCEINLINE void SetTranslation(const csVector4f &tr)
  {
    m30 = tr.x;
    m31 = tr.y;
    m32 = tr.z;
    m33 = tr.w;
  }


  CS_NODISCARD CS_FORCEINLINE csVector3f GetXAxis() const
  {
    return csVector3f(m00, m01, m02);
  }

  CS_NODISCARD CS_FORCEINLINE csVector3f GetYAxis() const
  {
    return csVector3f(m10, m11, m12);
  }


  CS_NODISCARD CS_FORCEINLINE csVector3f GetZAxis() const
  {
    return csVector3f(m20, m21, m22);
  }

  CS_NODISCARD CS_FORCEINLINE csVector3f GetAxis(eAxis axis) const
  {
    switch (axis)
    {
      case eAxis::X:
        return csVector3f(m00, m01, m02);
      case eAxis::Y:
        return csVector3f(m10, m11, m12);
      case eAxis::Z:
        return csVector3f(m20, m21, m22);
    }
    return csVector3f(0.0f, 0.0f, 0.0f);
  }


  CS_NODISCARD CS_FORCEINLINE csVector3f GetTranslation() const
  {
    return csVector3f(m30, m31, m32);
  }

  CS_FORCEINLINE void ExtractTRS(csVector3f &translation,
                                 csMatrix3f &rotation,
                                 csVector3f &scale)
  {
    translation = GetTranslation();
    scale       = csVector3f(GetXAxis().Length(),
                             GetYAxis().Length(),
                             GetZAxis().Length());

    rotation = csMatrix3f(m00 / scale.x, m01 / scale.x, m02 / scale.x,
                          m10 / scale.y, m11 / scale.y, m12 / scale.y,
                          m20 / scale.z, m21 / scale.z, m22 / scale.z
    );
  }

  CS_FORCEINLINE void ClearRotation()
  {
    m00 = m11 = m22 = 1.0f;
    m01 = m02 = m10 = m12 = m20 = m21 = 0.0f;
  }


  CS_FORCEINLINE void SetRotationX(float angle)
  {
    float c = std::cos(angle);
    float s = std::sin(angle);
    m11 = c;
    m12 = s;
    m21 = -s;
    m22 = c;
  }

  CS_FORCEINLINE void SetRotationY(float angle)
  {
    float c = std::cos(angle);
    float s = std::sin(angle);
    m00 = c;
    m02 = -s;
    m20 = s;
    m22 = c;
  }

  CS_FORCEINLINE void SetRotationZ(float angle)
  {
    float c = std::cos(angle);
    float s = std::sin(angle);
    m00 = c;
    m01 = s;
    m10 = -s;
    m11 = c;
  }

  CS_FORCEINLINE void SetScale(float x, float y, float z)
  {
    m00 = x;
    m11 = y;
    m22 = z;
  }

  CS_FORCEINLINE void SetScale(const csVector3f &s)
  {
    m00 = s.x;
    m11 = s.y;
    m22 = s.z;
  }

  CS_FORCEINLINE void Scale(const csVector3f &s)
  {
    m00 *= s.x;
    m11 *= s.y;
    m22 *= s.z;
  }

  CS_FORCEINLINE csMatrix4f &SetRotation(const csVector3f &axis, float angle)
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

  CS_NODISCARD CS_FORCEINLINE csMatrix4f Inverted() const
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

    return csMatrix4f(lm00, lm01, lm02, lm03,
                      lm10, lm11, lm12, lm13,
                      lm20, lm21, lm22, lm23,
                      lm30, lm31, lm32, lm33);
  }

  CS_FORCEINLINE void Invert()
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

  CS_NODISCARD CS_FORCEINLINE csMatrix4f Transposed() const
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
    return csMatrix4f(
        mm00, mm01, mm02, mm03,
        mm10, mm11, mm12, mm13,
        mm20, mm21, mm22, mm23,
        mm30, mm31, mm32, mm33
    );
  }


  CS_FORCEINLINE static csMatrix4f Translation(float x, float y, float z)
  {
    csMatrix4f m;
    m.SetIdentity();
    m.SetTranslation(x, y, z);
    return m;
  }

  CS_FORCEINLINE static csMatrix4f Translation(const csVector3f &tr)
  {
    csMatrix4f m;
    m.SetIdentity();
    m.SetTranslation(tr.x, tr.y, tr.z);
    return m;
  }


  CS_FORCEINLINE static csMatrix4f Rotation(float ax, float ay, float az, float angle)
  {
    csMatrix4f m;
    m.SetIdentity();
    m.SetRotation(csVector3f(ax, ay, az), angle);
    return m;
  }

  CS_FORCEINLINE static csMatrix4f Rotation(const csVector3f &axis, float angle)
  {
    csMatrix4f m;
    m.SetIdentity();
    m.SetRotation(axis, angle);

    return m;
  }


#define SWAP(a, b, s) (s) = (a); (a)=(b); (b)=(s)

  CS_FORCEINLINE void Transpose()
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

  CS_FORCEINLINE void SetLookAt(const csVector3f &eye, const csVector3f &spot, const csVector3f &up)
  {

    csVector3f zAxis = (spot - eye).Normalized();
    csVector3f xAxis = (up % zAxis).Normalized();
    csVector3f yAxis = zAxis % xAxis;
//    Vector3f::Sub(spot, eye, zAxis).Normalize();
//    Vector3f::Cross(up, zAxis, xAxis).Normalize();
//    Vector3f::Cross(zAxis, xAxis, yAxis);
    SetXAxis(xAxis.x, yAxis.x, zAxis.x);
    SetYAxis(xAxis.y, yAxis.y, zAxis.y);
    SetZAxis(xAxis.z, yAxis.z, zAxis.z);
    SetTranslation(-xAxis.Dot(eye), -yAxis.Dot(eye), -zAxis.Dot(eye));
  }

  CS_FORCEINLINE void SetLookAtInv(const csVector3f &eye, const csVector3f &spot, const csVector3f &up)
  {
//    Vector3f xAxis, yAxis, zAxis;
//    Vector3f::Sub(eye, spot, zAxis).Normalize();
//    Vector3f::Cross(up, zAxis, xAxis).Normalize();
//    Vector3f::Cross(zAxis, xAxis, yAxis);
//
//    Vector3f::Sub(spot, eye, zAxis).Normalize();
//    Vector3f::Cross(up, zAxis, xAxis).Normalize();
//    Vector3f::Cross(zAxis, xAxis, yAxis);

    csVector3f zAxis = (spot - eye).Normalized();
    csVector3f xAxis = (up % zAxis).Normalized();
    csVector3f yAxis = zAxis % xAxis;

    SetXAxis(xAxis);
    SetYAxis(yAxis);
    SetZAxis(zAxis);
    SetTranslation(eye);
  }

  CS_FORCEINLINE csMatrix4f &operator=(const csMatrix4f &other) = default;

  CS_FORCEINLINE friend csMatrix4f operator*(const csMatrix4f &m0, const csMatrix4f &m1)
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

    return csMatrix4f(
        mm00, mm01, mm02, mm03,
        mm10, mm11, mm12, mm13,
        mm20, mm21, mm22, mm23,
        mm30, mm31, mm32, mm33
    );
  }


  CS_FORCEINLINE friend csVector3f operator*(const csMatrix4f &m, const csVector3f &v)
  {
    float x = m.m00 * v.x + m.m10 * v.y + m.m20 * v.z;
    float y = m.m01 * v.x + m.m11 * v.y + m.m21 * v.z;
    float z = m.m02 * v.x + m.m12 * v.y + m.m22 * v.z;
    return csVector3f(x, y, z);
  }


  CS_NODISCARD CS_FORCEINLINE static csVector3f Mult(const csMatrix4f &m, const csVector3f &v)
  {
    float x = m.m00 * v.x + m.m10 * v.y + m.m20 * v.z;
    float y = m.m01 * v.x + m.m11 * v.y + m.m21 * v.z;
    float z = m.m02 * v.x + m.m12 * v.y + m.m22 * v.z;
    return csVector3f(x, y, z);
  }


  CS_FORCEINLINE friend csVector4f operator*(const csMatrix4f &m, const csVector4f &v)
  {
    float x = m.m00 * v.x + m.m10 * v.y + m.m20 * v.z + m.m30 * v.w;
    float y = m.m01 * v.x + m.m11 * v.y + m.m21 * v.z + m.m31 * v.w;
    float z = m.m02 * v.x + m.m12 * v.y + m.m22 * v.z + m.m32 * v.w;
    float w = m.m03 * v.x + m.m13 * v.y + m.m23 * v.z + m.m33 * v.w;
    return csVector4f(x, y, z, w);
  }


  CS_NODISCARD CS_FORCEINLINE static csVector4f Mult(const csMatrix4f &m, const csVector4f &v)
  {
    float x = m.m00 * v.x + m.m10 * v.y + m.m20 * v.z + m.m30 * v.w;
    float y = m.m01 * v.x + m.m11 * v.y + m.m21 * v.z + m.m31 * v.w;
    float z = m.m02 * v.x + m.m12 * v.y + m.m22 * v.z + m.m32 * v.w;
    float w = m.m03 * v.x + m.m13 * v.y + m.m23 * v.z + m.m33 * v.w;
    return csVector4f(x, y, z, w);
  }


  CS_NODISCARD CS_FORCEINLINE static csVector3f Transform(const csMatrix4f &m, const csVector3f &v)
  {
    float x = m.m00 * v.x + m.m10 * v.y + m.m20 * v.z + m.m30;
    float y = m.m01 * v.x + m.m11 * v.y + m.m21 * v.z + m.m31;
    float z = m.m02 * v.x + m.m12 * v.y + m.m22 * v.z + m.m32;
    return csVector3f(x, y, z);
  }

  CS_NODISCARD CS_FORCEINLINE static csVector2f TransformXY(const csMatrix4f &m, const csVector3f &v)
  {
    float x = m.m00 * v.x + m.m10 * v.y + m.m20 * v.z + m.m30;
    float y = m.m01 * v.x + m.m11 * v.y + m.m21 * v.z + m.m31;
    return csVector2f(x, y);
  }


  CS_NODISCARD CS_FORCEINLINE static float TransformZ(const csMatrix4f &m, const csVector3f &v)
  {
    return m.m02 * v.x + m.m12 * v.y + m.m22 * v.z + m.m32;
  }

  explicit CS_FORCEINLINE operator csMatrix3f() const
  {
    return csMatrix3f(
        m00, m01, m02,
        m10, m11, m12,
        m20, m21, m22
    );
  }

  CS_FORCEINLINE void Debug(const char *message = nullptr) const
  {
    if (message)
    {
      printf("cs::Matrix4f: %s\n", message);
    }
    else
    {
      printf("cs::Matrix4f:\n");
    }

    printf("  %.2f %.2f %.2f %.2f\n", m00, m01, m02, m03);
    printf("  %.2f %.2f %.2f %.2f\n", m10, m11, m12, m13);
    printf("  %.2f %.2f %.2f %.2f\n", m20, m21, m22, m23);
    printf("  %.2f %.2f %.2f %.2f\n", m30, m31, m32, m33);
    fflush(stdout);
  }


  CS_NODISCARD CS_FORCEINLINE bool IsIdentity() const
  {
    return CS_FLOAT_IS_ONE(m00) &&
           CS_FLOAT_IS_ONE(m11) &&
           CS_FLOAT_IS_ONE(m22) &&
           CS_FLOAT_IS_ONE(m33) &&
           CS_FLOAT_IS_ZERO(m01) &&
           CS_FLOAT_IS_ZERO(m02) &&
           CS_FLOAT_IS_ZERO(m03) &&
           CS_FLOAT_IS_ZERO(m10) &&
           CS_FLOAT_IS_ZERO(m12) &&
           CS_FLOAT_IS_ZERO(m13) &&
           CS_FLOAT_IS_ZERO(m20) &&
           CS_FLOAT_IS_ZERO(m21) &&
           CS_FLOAT_IS_ZERO(m23) &&
           CS_FLOAT_IS_ZERO(m30) &&
           CS_FLOAT_IS_ZERO(m31) &&
           CS_FLOAT_IS_ZERO(m32);
  }

};


}
