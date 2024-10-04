
#include <csCore/graphics/csVertexDeclaration.hh>
#include <csCore/graphics/csGraphicsExceptions.hh>
#include <csCore/math/csMath.hh>

namespace cryo
{

csVertexDeclaration::csVertexDeclaration(const std::vector<csVertexDeclaration::Attribute>& attributes)
{
  Init(&attributes[0], attributes.size());
}

csVertexDeclaration::csVertexDeclaration(const csVertexDeclaration::Attribute* attributes, Size count)
{
  Init(attributes, count);
}

void csVertexDeclaration::Init(const csVertexDeclaration::Attribute* attributes, Size count)
{
  uint8_t maxStream = 0;
  const Attribute* aptr = attributes;
  for (Size i = 0; i < count; i++, aptr++)
  {
    maxStream = ceMax(aptr->Stream, maxStream);
  }

  m_streams.clear();
  for (uint8_t streamId = 0; streamId <= maxStream; ++streamId)
  {
    m_streams.push_back(Stream());
    Stream& stream = m_streams[streamId];
    stream.StreamID = streamId;


    aptr = attributes;
    for (Size i = 0; i < count; i++, aptr++)
    {
      if (aptr->Stream == streamId)
      {
        stream.Attributes.push_back(*aptr);
      }
    }
  }

}

const std::vector<csVertexDeclaration::Attribute>& csVertexDeclaration::GetAttributes(uint8_t streamID) const
{
  if (streamID >= m_streams.size())
  {
    throw csNoSuchStreamException(streamID);
  }
  return m_streams[streamID].Attributes;

}

}