//
// Created by Marcell on 07.05.2024.
//

#include <ceCore/fps.hh>
#include <ceCore/ticker.hh>

namespace ce
{
FPS::FPS()
    : m_nextFrame(Ticker::Tick() + 1000)
    , m_lastFrame(Ticker::Tick())
{

}

int64_t FPS::Tick()
{
  m_count++;
  int64_t time = Ticker::Tick();
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

uint32_t FPS::Get() const
{
  return m_fps;
}



} // ce