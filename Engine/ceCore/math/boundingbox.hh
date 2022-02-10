#pragma once


#include <ceCore/coreexport.hh>
#include <ceCore/math/matrix4f.hh>
#include <ceCore/math/vector3f.hh>

namespace ce {

class CE_CORE_API BoundingBox
{
public:
  BoundingBox();
  ~BoundingBox() = default;

  void Add(const Vector3f& pos);
  void Add(const BoundingBox& bbox);

  void Add(const Matrix4f& M, const Vector3f& pos);
  void Add(const Matrix4f& M, const BoundingBox& bbox);

  void Clear();
  void Finish();

  CE_NODISCARD CE_FORCEINLINE bool IsValid() const
  {
    return m_valid;
  }

  CE_NODISCARD CE_FORCEINLINE const Vector3f& GetMin() const
  {
    return m_min;
  }

  CE_NODISCARD CE_FORCEINLINE const Vector3f& GetMax() const
  {
    return m_max;
  }
  CE_NODISCARD CE_FORCEINLINE const Vector3f& GetCenter() const
  {
    return m_center;
  }

  CE_NODISCARD CE_FORCEINLINE const Vector3f* GetPoints() const
  {
    return m_points;
  }

  CE_NODISCARD CE_FORCEINLINE float GetDiagonal() const
  {
    return m_diagonal;
  }

  void Debug(const char* message = nullptr) const;

private:
  bool m_valid;

  Vector3f m_min;
  Vector3f m_max;
  Vector3f m_center;

  Vector3f m_points[8];
  float m_diagonal;
};




}
