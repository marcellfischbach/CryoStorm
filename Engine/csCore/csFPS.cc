//
// Created by Marcell on 07.05.2024.
//

#include <csCore/csFPS.hh>
#include <csCore/csTicker.hh>

namespace cryo
{
csFPS::csFPS()
    : m_nextFrame(csTicker::Tick() + 1000)
    , m_lastFrame(csTicker::Tick())
{

}

int64_t csFPS::Tick()
{
  m_count++;
  int64_t time = csTicker::Tick();
  if (time >= m_nextFrame)
  {
    while (time >= m_nextFrame)
    {
      m_nextFrame += 1000;
    }
    m_fps = m_count;
    m_count = 0;
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