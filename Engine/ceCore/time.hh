
#pragma once
#include <ceCore/coreexport.hh>
#include <ceCore/types.hh>
#include <stdint.h>

namespace ce
{

class CS_CORE_API Time
{
public:
  static uint64_t GetTime ();
private:
  Time () = default;
};

} // ce
