//
// Created by Marcell on 07.05.2024.
//

#include <csCore/csFPS.hh>
#include <csCore/csTicker.hh>

namespace cs
{
csFPS::csFPS()
    : m_nextFrame(csTicker::Tick() + 1000)
    , m_lastFrame(csTicker::Tick())
{

}

static uint64_t s_totalFps = 0;
static uint64_t s_totalSecs = 0;


int64_t csFPS::Tick()
{
  m_count++;
  int64_t time = csTicker::Tick();
  if (time >= m_nextFrame)
  {
    while (time >= m_nextFrame)
    {
      s_totalSecs++;
      m_nextFrame += 1000;
    }

    s_totalFps += m_count;
    m_fps = m_count;
    m_count = 0;

    printf ("Total FPS: %llu\n", s_totalFps / s_totalSecs);
    fflush(stdout);
  }

  int64_t delta = time - m_lastFrame;
  m_lastFrame = time;
  return delta;
}

uint32_t csFPS::Get() const
{
  return m_fps;
}



} // ce