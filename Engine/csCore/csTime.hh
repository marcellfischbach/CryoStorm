
#pragma once
#include <csCore/csCoreExport.hh>
#include <csCore/csTypes.hh>
#include <stdint.h>

namespace cs
{

class CS_CORE_API csTime
{
public:
  static uint64_t GetTime ();
private:
  csTime() = default;
};

} // ce
