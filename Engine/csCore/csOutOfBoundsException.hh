
#pragma once

#include <csCore/csCoreExport.hh>
#include <exception>

namespace cs
{

class csOutOfBoundsException : public std::exception
{
public:
	inline csOutOfBoundsException() : std::exception() { }
};

}