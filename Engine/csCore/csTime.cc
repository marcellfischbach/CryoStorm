
#include <csCore/csTime.hh>
#ifdef CS_WIN32
#include <windows.h>
static const uint64_t SECS_IN_MS = 1000;
static const uint64_t MIN_IN_MS = 60 * SECS_IN_MS;
static const uint64_t HOURS_IN_MS = 60 * MIN_IN_MS;
#endif

namespace cs
{



uint64_t csTime::GetTime()
{
#ifdef CS_WIN32

  LARGE_INTEGER ticks, frequency;

  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&ticks);


  return (ticks.QuadPart * 1000) / frequency.QuadPart;
#endif
}

} // ce