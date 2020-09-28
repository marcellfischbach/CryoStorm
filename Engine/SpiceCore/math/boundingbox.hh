#pragma once


#include <SpiceCore/coreexport.hh>
#include <SpiceCore/math/matrix4f.hh>
#include <SpiceCore/math/vector3f.hh>

namespace Spice {

class SPICE_CORE_API BoundingBox
{
public:
  BoundingBox();
  ~BoundingBox();

  void Add(const Vector3f& pos);
  void Add(const BoundingBox& bbox);

  void Add(const Matrix4f& M, const Vector3f& pos);
  void Add(const Matrix4f& M, const BoundingBox& bbox);

  void Clear();
  void Finish();

  SPICE_FORCEINLINE bool IsValid() const
  {
    return m_valid;
  }

  SPICE_FORCEINLINE const Vector3f& GetMin() const
  {
    return m_min;
  }

  SPICE_FORCEINLINE const Vector3f& GetMax() const
  {
    return m_max;
  }
  SPICE_FORCEINLINE const Vector3f& GetCenter() const
  {
    return m_center;
  }

  SPICE_FORCEINLINE const Vector3f* GetPoints() const
  {
    return m_points;
  }

  void Debug(const char* message = 0) const;

private:
  bool m_valid;

  Vector3f m_min;
  Vector3f m_max;
  Vector3f m_center;

  Vector3f m_points[8];

};




}
