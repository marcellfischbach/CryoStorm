
#pragma once

#include <spcCore/coreexport.hh>

#include <spcCore/math/quaternion.hh>
#include <spcCore/math/vector3f.hh>

namespace spc
{

class SPC_CORE_API Transform
{
public:
  Transform();
  
  

private:
  Vector3f m_translation;
  Quaternion m_rotation;
  Vector3f m_scale;
};

}