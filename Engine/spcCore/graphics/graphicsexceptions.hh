
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
  inline NoSuchStreamException(UInt8 streamId) : std::exception()
    , m_streamId(streamId)
  {

  }

  inline UInt8 GetStreamId() const { return m_streamId;  }

private:
  UInt8 m_streamId;
};


}