#pragma once

#include <ceCore/bitmask.hh>

namespace ce
{

enum class eSGValueType
{
  Invalid   = 0x0000,
  Scalar    = 0x0001,
  Vector2   = 0x0002,
  Vector3   = 0x0004,
  Vector4   = 0x0008,
  Color4    = 0x0010,
} CE_ENABLE_BITMASK(eSGValueType);


}