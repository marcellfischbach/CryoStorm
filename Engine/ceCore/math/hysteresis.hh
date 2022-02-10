#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/defs.hh>

namespace ce
{

template<typename T>
class CE_CORE_API Hysteresis
{
public:
  CE_FORCEINLINE Hysteresis()
  {

  }

  CE_FORCEINLINE void Setup(const& T low, const& T high, const& T value)
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

  CE_FORCEINLINE bool UpdateValue(const& T value)
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
  CE_NODISCARD CE_FORCEINLINE bool IsLow() const
  {
    return m_state == Low;
  }

  CE_NODISCARD CE_FORCEINLINE bool IsHigh() const
  {
    return m_state == Hight;
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
