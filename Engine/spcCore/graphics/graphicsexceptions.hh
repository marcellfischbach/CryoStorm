
#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/types.hh>
#include <exception>
#include <string>

namespace spc
{

class NoSuchStreamException : public std::exception
{
public:
  inline NoSuchStreamException(uint8_t streamId) : std::exception()
    , m_streamId(streamId)
  {

  }

  inline uint8_t GetStreamId() const { return m_streamId;  }

private:
  uint8_t m_streamId;
};


}