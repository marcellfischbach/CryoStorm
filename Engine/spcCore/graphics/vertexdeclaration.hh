

#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/types.hh>
#include <spcCore/graphics/datatype.hh>
#include <spcCore/graphics/vertexstream.hh>
#include <vector>

namespace spc
{



class SPC_CORE_API VertexDeclaration
{
public:
  struct Attribute 
  {
    UInt8 Stream;
    eVertexStream Location;
    UInt8 Size;
    eDataType Type;
    UInt16 Stride;
    UInt16 Offset;
  };

  VertexDeclaration(const Attribute* attributes, Size count);



};


}