#pragma once

#include <SpiceCore/coreexport.hh>
#include <SpiceCore/defs.hh>

namespace Spice
{

template<typename T>
class SPICE_CORE_API Hysteresis
{
public:
  SPICE_FORCEINLINE Hysteresis()
  {

  }

  SPICE_FORCEINLINE void Setup(const& T low, const& T high, const& T value)
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

  SPICE_FORCEINLINE bool UpdateValue(const& T value)
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
  SPICE_FORCEINLINE bool IsLow() const
  {
    return m_state == Low;
  }

  SPICE_FORCEINLINE bool IsHigh() const
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
