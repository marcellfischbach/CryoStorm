

#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csTypes.hh>
#include <csCore/graphics/eDataType.hh>
#include <csCore/graphics/eVertexStream.hh>
#include <vector>
#include <csCore/csDefs.hh>

namespace cs
{



class CS_CORE_API csVertexDeclaration
{
public:
  struct Attribute 
  {
    uint8_t Stream;
    eVertexStream Location;
    uint8_t Size;
    eDataType Type;
    uint16_t Stride;
    uint16_t Offset;
    Attribute() {}
    Attribute(uint8_t stream, eVertexStream location, uint8_t size, eDataType type, uint16_t stride, uint16_t offset)
      : Stream(stream)
      , Location(location)
      , Size(size)
      , Type(type)
      , Stride(stride)
      , Offset(offset)
    {}
  };


  csVertexDeclaration() = default;
  csVertexDeclaration(const Attribute* attributes, Size count);
  explicit csVertexDeclaration(const std::vector<Attribute>& attributes);

  CS_NODISCARD const std::vector<Attribute>& GetAttributes(uint8_t streamID) const;

private:
  void Init(const Attribute* attributes, Size count);
  struct Stream 
  {
    uint8_t StreamID;
    std::vector<Attribute> Attributes;
  };

  std::vector<Stream> m_streams;
};


}