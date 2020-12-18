
#pragma once

#include <spcCore/coreexport.hh>
#include <exception>

namespace spc
{

class SPC_CORE_API OutOfBoundsException : public std::exception
{
public:
	OutOfBoundsException();
};

}