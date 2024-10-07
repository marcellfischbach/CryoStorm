
#include <csCore/csTicker.hh>

namespace cs
{

#ifdef CS_WIN32

#include <Windows.h>


int64_t csTicker::Tick()
{
  LARGE_INTEGER c, f;
  if (QueryPerformanceCounter(&c) && QueryPerformanceFrequency(&f))
  {
    c.QuadPart *= 1000000;
    c.QuadPart /= f.QuadPart;
    c.QuadPart /= 1000;
    return c.QuadPart;
  }

  return GetTickCount64();
}

#endif


} // ce