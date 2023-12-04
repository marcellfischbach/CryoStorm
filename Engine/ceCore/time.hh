
#pragma once
#include <ceCore/coreexport.hh>
#include <ceCore/types.hh>

namespace ce
{

class CE_CORE_API Time
{
public:
  static uint64_t GetTime ();
private:
  Time () = default;
};

} // ce
