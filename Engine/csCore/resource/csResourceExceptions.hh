
#pragma once
#include <csCore/csCoreExport.hh>
#include <exception>

namespace cs
{

class csFileNotOpenException : public std::exception
{
public:
  inline csFileNotOpenException() : std::exception() { }
};

}