
#pragma once
#include <ceCore/coreexport.hh>
#include <exception>

namespace ce
{

class FileNotOpenException : public std::exception
{
public:
  inline FileNotOpenException() : std::exception() { }
};

}