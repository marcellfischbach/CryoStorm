#pragma once

#include <SpiceCore/coreexport.hh>
#include <string>

namespace Spice
{

class SPICE_CORE_API ResourceLocator
{
public:
	ResourceLocator(const std::string &encoded);

	const std::string& GetLocator() const;

private:
	std::string m_locator;

};

}
