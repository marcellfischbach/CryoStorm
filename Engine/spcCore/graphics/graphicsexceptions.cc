
#include <spcCore/graphics/graphicsexceptions.hh>

namespace spc
{


NoSuchStreamException::NoSuchStreamException(UInt8 streamId)
  : std::exception()
  , m_streamId(streamId)
{

}

UInt8 NoSuchStreamException::GetStreamId() const
{
  return m_streamId;
}


}