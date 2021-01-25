
#pragma once

#include <spcCore/coreexport.hh>
#include <exception>

namespace spc
{

class OutOfBoundsException : public std::exception
{
public:
	inline OutOfBoundsException() : std::exception() { }
};

}