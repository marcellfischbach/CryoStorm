#pragma once


#include <cecore/ceexport.hh>
#include <cecore/math/cematrix4f.hh>
#include <cecore/math/cevector3f.hh>


class CEC_API ceBoundingBox
{
public:
  ceBoundingBox();
  ~ceBoundingBox();

  void Add(const ceVector3f& pos);
  void Add(const ceBoundingBox& bbox);

  void Add(const ceMatrix4f& M, const ceVector3f& pos);
  void Add(const ceMatrix4f& M, const ceBoundingBox& bbox);

  void Clear();
  void Finish();

  CE_FORCEINLINE bool IsValid() const
  {
    return m_valid;
  }

  CE_FORCEINLINE const ceVector3f& GetMin() const
  {
    return m_min;
  }

  CE_FORCEINLINE const ceVector3f& GetMax() const
  {
    return m_max;
  }
  CE_FORCEINLINE const ceVector3f& GetCenter() const
  {
    return m_center;
  }

  CE_FORCEINLINE const ceVector3f* GetPoints() const
  {
    return m_points;
  }

  void Debug(const char* message = 0) const;

private:
  bool m_valid;

  ceVector3f m_min;
  ceVector3f m_max;
  ceVector3f m_center;

  ceVector3f m_points[8];

};




