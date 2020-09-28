#pragma once

#include <cecore/ceexport.hh>
#include <stdlib.h>


class CEC_API ceRandomRange
{
public:
  ceRandomRange()
    : m_min(0)
    , m_range(0)
  {

  }

  void SetRange(float min, float max)
  {
    if (min > max)
    {
      float s = min;
      min = max;
      max = s;
    }
    m_min = min;
    m_range = max - min;
  }

  void SetValue(float value, float jitter = 0.0f)
  {
    m_min = value - jitter / 2.0f;
    m_range = jitter;
  }

  float Get() const
  {
    return m_min + (float)rand() / (float)RAND_MAX * m_range;
  }

  static ceRandomRange Range(float min, float max)
  {
    ceRandomRange rr;
    rr.SetRange(min, max);
    return rr;
  }

  static ceRandomRange Value(float value, float jitter = 0.0f)
  {
    ceRandomRange rr;
    rr.SetValue(value, jitter);
    return rr;
  }

  float GetMin() const
  {
    return m_min;
  }

  float GetRange() const
  {
    return m_range;
  }

private:
  float m_min;
  float m_range;
};

