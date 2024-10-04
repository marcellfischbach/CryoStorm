
#pragma once
#include <ceCore/csCoreExport.hh>
#include <ceCore/csTypes.hh>
#include <stdint.h>

namespace cryo
{

class CS_CORE_API csTime
{
public:
  static uint64_t GetTime ();
private:
  csTime() = default;
};

} // ce
