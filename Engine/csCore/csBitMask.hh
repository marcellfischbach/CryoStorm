
#pragma once

#include <type_traits>

#define CS_ENABLE_BITMASK(x)         \
;template<>                           \
struct csEnableBitMaskOperators<x> \
{                                    \
    static const bool enable = true; \
}

namespace cs
{


template<typename TEnum>
struct csEnableBitMaskOperators
{
  static const bool enable = false;
};

template<typename TEnum>
typename std::enable_if<csEnableBitMaskOperators<TEnum>::enable, TEnum>::type
operator |(TEnum lhs, TEnum rhs)
{
  return static_cast<TEnum> (
      static_cast<typename std::underlying_type<TEnum>::type>(lhs) |
      static_cast<typename std::underlying_type<TEnum>::type>(rhs)
  );
}
template<typename TEnum>
typename std::enable_if<csEnableBitMaskOperators<TEnum>::enable, TEnum>::type
operator &(TEnum lhs, TEnum rhs)
{
  return static_cast<TEnum> (
      static_cast<typename std::underlying_type<TEnum>::type>(lhs) &
      static_cast<typename std::underlying_type<TEnum>::type>(rhs)
  );
}

template<typename TEnum>
typename std::enable_if<csEnableBitMaskOperators<TEnum>::enable, TEnum>::type
operator ^(TEnum lhs, TEnum rhs)
{
  return static_cast<TEnum> (
      static_cast<typename std::underlying_type<TEnum>::type>(lhs) ^
      static_cast<typename std::underlying_type<TEnum>::type>(rhs)
  );
}

template<typename TEnum>
typename std::enable_if<csEnableBitMaskOperators<TEnum>::enable, TEnum>::type
operator ~(TEnum rhs)
{
  return static_cast<TEnum> (
      ~static_cast<typename std::underlying_type<TEnum>::type>(rhs)
  );
}

template<typename TEnum>
typename std::enable_if<csEnableBitMaskOperators<TEnum>::enable, bool>::type
operator ==(TEnum lhs, TEnum rhs)
{
  return
    static_cast<typename std::underlying_type<TEnum>::type>(lhs) ==
    static_cast<typename std::underlying_type<TEnum>::type>(rhs);

}

template<typename TEnum>
typename std::enable_if<csEnableBitMaskOperators<TEnum>::enable, TEnum>::type&
operator |=(TEnum &lhs, TEnum rhs)
{
  lhs = static_cast<TEnum> (
      static_cast<typename std::underlying_type<TEnum>::type>(lhs) |
      static_cast<typename std::underlying_type<TEnum>::type>(rhs)
  );

  return lhs;
}

template<typename TEnum>
typename std::enable_if<csEnableBitMaskOperators<TEnum>::enable, TEnum>::type&
operator &=(TEnum &lhs, TEnum rhs)
{
  lhs = static_cast<TEnum> (
      static_cast<typename std::underlying_type<TEnum>::type>(lhs) &
      static_cast<typename std::underlying_type<TEnum>::type>(rhs)
  );

  return lhs;
}

template<typename TEnum>
typename std::enable_if<csEnableBitMaskOperators<TEnum>::enable, TEnum>::type&
operator ^=(TEnum &lhs, TEnum rhs)
{
  lhs = static_cast<TEnum> (
      static_cast<typename std::underlying_type<TEnum>::type>(lhs) ^
      static_cast<typename std::underlying_type<TEnum>::type>(rhs)
  );

  return lhs;
}

}