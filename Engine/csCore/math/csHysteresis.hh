#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csDefs.hh>

namespace cs
{

template<typename T>
class CS_CORE_API csHysteresis
{
public:
  CS_FORCEINLINE csHysteresis()
  {

  }

  CS_FORCEINLINE void Setup(const T &low, const T &high, const T &value)
  {
    m_low = low;
    m_high = high;
    if (value < (low + high) / 2)
    {
      m_state = Low;
    }
    else
    {
      m_state = High;
    }
  }

  CS_FORCEINLINE bool UpdateValue(const T &value)
  {
    m_value = value;
    switch (m_state)
    {
    case Low:
      if (m_value > m_high)
      {
        m_state = High;
        return true;
      }
    case High:
      if (m_value < m_low)
      {
        m_state = Low;
        return true;
      }
    }
    return false;
  }

  CS_NODISCARD CS_FORCEINLINE bool IsLow() const
  {
    return m_state == Low;
  }

  CS_NODISCARD CS_FORCEINLINE bool IsHigh() const
  {
    return m_state == High;
  }

private:
  T m_low;
  T m_high;
  T m_value;

  enum State
  {
    Low,
    High
  } m_state;
};




}
