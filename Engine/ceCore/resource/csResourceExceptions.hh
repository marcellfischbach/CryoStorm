
#pragma once
#include <ceCore/csCoreExport.hh>
#include <exception>

namespace cryo
{

class csFileNotOpenException : public std::exception
{
public:
  inline csFileNotOpenException() : std::exception() { }
};

}