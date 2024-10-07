#pragma once

namespace cs
{

enum eMaterialAttributeType
{
  eMAT_Float,
  eMAT_Vec2,
  eMAT_Vec3,
  eMAT_Vec4,
  eMAT_Int,
  eMAT_IVec2,
  eMAT_IVec3,
  eMAT_IVec4,
  eMAT_Matrix3,
  eMAT_Matrix4,
  eMAT_Texture,

  eMAT_Undefined,
  eMAT_COUNT = eMAT_Undefined,
};

}