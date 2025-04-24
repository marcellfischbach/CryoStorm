//
// Created by MCEL on 24.04.2025.
//

#include <csCore/animation/csSkeletonAnimationProperties.hh>

namespace cs
{


uint32_t csSkeletonAnimationProperties::IndexOf(const std::string &propertyName)
{
  for (int32_t i = 0, in = m_properties.size(); i < in; i++)
  {
    if (m_properties[i].name == propertyName)
    {
      return i;
    }
  }
  return INVALID_INDEX;
}


csSkeletonAnimationProperties::eType csSkeletonAnimationProperties::GetType(uint32_t propIndex) const
{
  if (propIndex >= m_properties.size())
  {
    return PT_INVALID;
  }

  return m_properties[propIndex].type;
}


bool csSkeletonAnimationProperties::GetBoolValue(cs::uint32_t propIndex) const
{
  if (propIndex >= m_properties.size())
  {
    return false;
  }

  const Property &prop = m_properties[propIndex];
  if (prop.type != PT_BOOL)
  {
    return false;
  }

  return prop.boolValue;
}

float csSkeletonAnimationProperties::GetFloatValue(cs::uint32_t propIndex) const
{
  if (propIndex >= m_properties.size())
  {
    return 0.0f;
  }

  const Property &prop = m_properties[propIndex];
  if (prop.type != PT_FLOAT)
  {
    return 0.0f;
  }

  return prop.floatValue;
}

const csVector2f &csSkeletonAnimationProperties::GetVec2Value(cs::uint32_t propIndex) const
{
  if (propIndex >= m_properties.size())
  {
    return m_vec2Null;
  }

  const Property &prop = m_properties[propIndex];
  if (prop.type != PT_VEC2)
  {
    return m_vec2Null;
  }

  return prop.vec2Value;
}


const csVector3f &csSkeletonAnimationProperties::GetVec3Value(cs::uint32_t propIndex) const
{
  if (propIndex >= m_properties.size())
  {
    return m_vec3Null;
  }

  const Property &prop = m_properties[propIndex];
  if (prop.type != PT_VEC3)
  {
    return m_vec3Null;
  }

  return prop.vec3Value;
}


const csVector4f &csSkeletonAnimationProperties::GetVec4Value(cs::uint32_t propIndex) const
{
  if (propIndex >= m_properties.size())
  {
    return m_vec4Null;
  }

  const Property &prop = m_properties[propIndex];
  if (prop.type != PT_VEC4)
  {
    return m_vec4Null;
  }

  return prop.vec4Value;
}


int32_t csSkeletonAnimationProperties::GetIntValue(cs::uint32_t propIndex) const
{
  if (propIndex >= m_properties.size())
  {
    return 0;
  }

  const Property &prop = m_properties[propIndex];
  if (prop.type != PT_INT)
  {
    return 0;
  }

  return prop.intValue;
}


void csSkeletonAnimationProperties::SetProperty(cs::uint32_t propIndex, bool boolValue)
{
  if (propIndex >= m_properties.size())
  {
    return;
  }

  Property &prop = m_properties[propIndex];
  if (prop.type != PT_BOOL)
  {
    return;
  }
  prop.boolValue = boolValue;
}

void csSkeletonAnimationProperties::SetProperty(cs::uint32_t propIndex, float floatValue)
{
  if (propIndex >= m_properties.size())
  {
    return;
  }

  Property &prop = m_properties[propIndex];
  if (prop.type != PT_FLOAT)
  {
    return;
  }
  prop.floatValue = floatValue;
}

void csSkeletonAnimationProperties::SetProperty(cs::uint32_t propIndex, const csVector2f &vecValue)
{
  if (propIndex >= m_properties.size())
  {
    return;
  }

  Property &prop = m_properties[propIndex];
  if (prop.type != PT_VEC2)
  {
    return;
  }
  prop.vec2Value = vecValue;
}


void csSkeletonAnimationProperties::SetProperty(cs::uint32_t propIndex, const csVector3f &vecValue)
{
  if (propIndex >= m_properties.size())
  {
    return;
  }

  Property &prop = m_properties[propIndex];
  if (prop.type != PT_VEC3)
  {
    return;
  }
  prop.vec3Value = vecValue;
}


void csSkeletonAnimationProperties::SetProperty(cs::uint32_t propIndex, const csVector4f &vecValue)
{
  if (propIndex >= m_properties.size())
  {
    return;
  }

  Property &prop = m_properties[propIndex];
  if (prop.type != PT_VEC4)
  {
    return;
  }
  prop.vec4Value = vecValue;
}


void csSkeletonAnimationProperties::SetProperty(cs::uint32_t propIndex, int32_t intValue)
{
  if (propIndex >= m_properties.size())
  {
    return;
  }

  Property &prop = m_properties[propIndex];
  if (prop.type != PT_INT)
  {
    return;
  }
  prop.intValue = intValue;
}


uint32_t csSkeletonAnimationProperties::DeclareProperty(const std::string &propertyName,
                                                        cs::csSkeletonAnimationProperties::eType propertyType)
{
  for (int32_t i = 0, in = m_properties.size(); i < in; i++)
  {
    if (m_properties[i].name == propertyName)
    {
      if (m_properties[i].type == propertyType)
      {
        return i;
      }
      return INVALID_INDEX;
    }
  }

  m_properties.emplace_back(propertyName,
                            propertyType,
                            false,
                            0.0f,
                            csVector2f(),
                            csVector3f(),
                            csVector4f(),
                            0);
  return m_properties.size()-1;
}


} // cs