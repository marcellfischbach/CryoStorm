
#pragma once

#include <csCore/csCoreExport.hh>
#include <exception>

namespace cryo
{

class csOutOfBoundsException : public std::exception
{
public:
	inline csOutOfBoundsException() : std::exception() { }
};

}