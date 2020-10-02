
#include <spcCore/graphics/vertexdeclaration.hh>
#include <spcCore/graphics/graphicsexceptions.hh>
#include <spcCore/math/math.hh>

namespace spc
{

VertexDeclaration::VertexDeclaration(const std::vector<VertexDeclaration::Attribute>& attributes)
{
  Init(&attributes[0], attributes.size());
}

VertexDeclaration::VertexDeclaration(const VertexDeclaration::Attribute* attributes, Size count)
{
  Init(attributes, count);
}

void VertexDeclaration::Init(const VertexDeclaration::Attribute* attributes, Size count)
{
  UInt8 maxStream = 0;
  const Attribute* aptr = attributes;
  for (Size i = 0; i < count; i++, aptr++)
  {
    maxStream = spcMax(aptr->Stream, maxStream);
  }

  m_streams.clear();
  for (UInt8 streamId = 0; streamId <= maxStream; ++streamId)
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

const std::vector<VertexDeclaration::Attribute>& VertexDeclaration::GetAttributes(UInt8 streamID) const
{
  if (streamID >= m_streams.size())
  {
    throw NoSuchStreamException(streamID);
  }
  return m_streams[streamID].Attributes;

}

}