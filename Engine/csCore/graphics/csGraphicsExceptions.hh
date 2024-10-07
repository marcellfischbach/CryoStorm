
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csTypes.hh>
#include <exception>
#include <string>

namespace cs
{

class csNoSuchStreamException : public std::exception
{
public:
  inline csNoSuchStreamException(uint8_t streamId) : std::exception()
    , m_streamId(streamId)
  {

  }

  inline uint8_t GetStreamId() const { return m_streamId;  }

private:
  uint8_t m_streamId;
};


}