
#pragma once

#include <type_traits>

#define CS_ENABLE_BITMASK(x)         \
;template<>                           \
struct EnableBitMaskOperators<x> \
{                                    \
    static const bool enable = true; \
}

namespace ce
{


template<typename TEnum>
struct EnableBitMaskOperators
{
  static const bool enable = false;
};

template<typename TEnum>
typename std::enable_if<EnableBitMaskOperators<TEnum>::enable, TEnum>::type
operator |(TEnum lhs, TEnum rhs)
{
  return static_cast<TEnum> (
      static_cast<typename std::underlying_type<TEnum>::type>(lhs) |
      static_cast<typename std::underlying_type<TEnum>::type>(rhs)
  );
}
template<typename TEnum>
typename std::enable_if<EnableBitMaskOperators<TEnum>::enable, TEnum>::type
operator &(TEnum lhs, TEnum rhs)
{
  return static_cast<TEnum> (
      static_cast<typename std::underlying_type<TEnum>::type>(lhs) &
      static_cast<typename std::underlying_type<TEnum>::type>(rhs)
  );
}

template<typename TEnum>
typename std::enable_if<EnableBitMaskOperators<TEnum>::enable, TEnum>::type
operator ^(TEnum lhs, TEnum rhs)
{
  return static_cast<TEnum> (
      static_cast<typename std::underlying_type<TEnum>::type>(lhs) ^
      static_cast<typename std::underlying_type<TEnum>::type>(rhs)
  );
}

template<typename TEnum>
typename std::enable_if<EnableBitMaskOperators<TEnum>::enable, TEnum>::type
operator ~(TEnum rhs)
{
  return static_cast<TEnum> (
      ~static_cast<typename std::underlying_type<TEnum>::type>(rhs)
  );
}

template<typename TEnum>
typename std::enable_if<EnableBitMaskOperators<TEnum>::enable, bool>::type
operator ==(TEnum lhs, TEnum rhs)
{
  return
    static_cast<typename std::underlying_type<TEnum>::type>(lhs) ==
    static_cast<typename std::underlying_type<TEnum>::type>(rhs);

}

template<typename TEnum>
typename std::enable_if<EnableBitMaskOperators<TEnum>::enable, TEnum>::type&
operator |=(TEnum &lhs, TEnum rhs)
{
  lhs = static_cast<TEnum> (
      static_cast<typename std::underlying_type<TEnum>::type>(lhs) |
      static_cast<typename std::underlying_type<TEnum>::type>(rhs)
  );

  return lhs;
}

template<typename TEnum>
typename std::enable_if<EnableBitMaskOperators<TEnum>::enable, TEnum>::type&
operator &=(TEnum &lhs, TEnum rhs)
{
  lhs = static_cast<TEnum> (
      static_cast<typename std::underlying_type<TEnum>::type>(lhs) &
      static_cast<typename std::underlying_type<TEnum>::type>(rhs)
  );

  return lhs;
}

template<typename TEnum>
typename std::enable_if<EnableBitMaskOperators<TEnum>::enable, TEnum>::type&
operator ^=(TEnum &lhs, TEnum rhs)
{
  lhs = static_cast<TEnum> (
      static_cast<typename std::underlying_type<TEnum>::type>(lhs) ^
      static_cast<typename std::underlying_type<TEnum>::type>(rhs)
  );

  return lhs;
}

}