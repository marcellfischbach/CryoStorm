
#pragma once
#include <spcCore/coreexport.hh>
#include <exception>

namespace spc
{

class FileNotOpenException : public std::exception
{
public:
  inline FileNotOpenException() : std::exception() { }
};

}