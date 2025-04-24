//
// Created by MCEL on 24.04.2025.
//

#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csTypes.hh>
#include <csCore/math/csVector.hh>
#include <string>
#include <vector>

namespace cs
{

class CS_CORE_API csSkeletonAnimationProperties
{
public:
  static const uint32_t INVALID_INDEX = ~0x00;

  enum eType
  {
    PT_BOOL,
    PT_FLOAT,
    PT_VEC2,
    PT_VEC3,
    PT_VEC4,
    PT_INT,
    PT_INVALID,
  };


  /**
   * Returns the index of the property with the given name. If no such property is declared it returns INVALID_INDEX
   */
  uint32_t IndexOf(const std::string &propertyName);

  eType GetType (uint32_t propIndex) const;
  bool GetBoolValue (uint32_t propIndex) const;
  float GetFloatValue (uint32_t propIndex) const;
  const csVector2f &GetVec2Value (uint32_t propIndex) const;
  const csVector3f &GetVec3Value (uint32_t propIndex) const;
  const csVector4f &GetVec4Value (uint32_t propIndex) const;
  int32_t GetIntValue (uint32_t propIndex) const;

  void SetProperty (uint32_t propIndex, bool boolValue);
  void SetProperty (uint32_t propIndex, float floatValue);
  void SetProperty (uint32_t propIndex, const csVector2f &vec2Value);
  void SetProperty (uint32_t propIndex, const csVector3f &vec3Value);
  void SetProperty (uint32_t propIndex, const csVector4f &vec4Value);
  void SetProperty (uint32_t propIndex, int32_t intValue);


  /**
   * Returns the index of the declared property. If the property already existed the index of the already declared
   * property is returned. If the type of the already declared property differs from the new declaration INVALID_INDEX
   * will be returned.
   */
  uint32_t DeclareProperty (const std::string &propertyName, eType propertyType);

private:

  struct Property
  {
    std::string name;
    eType       type;
    bool        boolValue;
    float       floatValue;
    csVector2f  vec2Value;
    csVector3f  vec3Value;
    csVector4f  vec4Value;
    int32_t     intValue;
  };

  std::vector<Property> m_properties;
  csVector2f m_vec2Null;
  csVector3f m_vec3Null;
  csVector4f m_vec4Null;

};

} // cs
