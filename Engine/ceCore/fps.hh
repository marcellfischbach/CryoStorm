//
// Created by Marcell on 07.05.2024.
//

#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/types.hh>

namespace cryo
{

class CS_CORE_API FPS
{
public:
  FPS();

  int64_t Tick ();
  uint32_t Get() const;

private:
  int64_t m_nextFrame;
  int64_t m_lastFrame;

  uint32_t m_count = 0;
  uint32_t m_fps = 0;
};

} // ce
