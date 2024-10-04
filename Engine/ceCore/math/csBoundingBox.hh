#pragma once


#include <ceCore/csCoreExport.hh>
#include <ceCore/math/csMatrix4f.hh>
#include <ceCore/math/csVector3f.hh>

namespace cryo {

class CS_CORE_API csBoundingBox
{
public:
  csBoundingBox();
  ~csBoundingBox() = default;

  void Add(const csVector3f& pos);
  void Add(const csBoundingBox& bbox);

  void Add(const csMatrix4f& M, const csVector3f& pos);
  void Add(const csMatrix4f& M, const csBoundingBox& bbox);

  void Clear();
  void Finish();

  CS_NODISCARD CS_FORCEINLINE bool IsValid() const
  {
    return m_valid;
  }

  CS_NODISCARD CS_FORCEINLINE const csVector3f& GetMin() const
  {
    return m_min;
  }

  CS_NODISCARD CS_FORCEINLINE const csVector3f& GetMax() const
  {
    return m_max;
  }
  CS_NODISCARD CS_FORCEINLINE const csVector3f& GetCenter() const
  {
    return m_center;
  }

  CS_NODISCARD CS_FORCEINLINE const csVector3f* GetPoints() const
  {
    return m_points;
  }

  CS_NODISCARD CS_FORCEINLINE float GetDiagonal() const
  {
    return m_diagonal;
  }

  void Debug(const char* message = nullptr) const;

private:
  bool m_valid;

  csVector3f m_min;
  csVector3f m_max;
  csVector3f m_center;

  csVector3f m_points[8];
  float      m_diagonal;
};




}
