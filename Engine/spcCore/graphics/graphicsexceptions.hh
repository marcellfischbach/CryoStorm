
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
  NoSuchStreamException(UInt8 streamId);

    UInt8 GetStreamId() const;

private:
  UInt8 m_streamId;
};


}