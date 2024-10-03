
#pragma once

#include <ceCore/coreexport.hh>
#include <exception>

namespace cryo
{

class OutOfBoundsException : public std::exception
{
public:
	inline OutOfBoundsException() : std::exception() { }
};

}