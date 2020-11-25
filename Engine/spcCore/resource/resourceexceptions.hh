
#pragma once
#include <spcCore/coreexport.hh>
#include <exception>

namespace spc
{

class SPC_CORE_API FileNotOpenException : public std::exception
{
public:
  FileNotOpenException();
};

}