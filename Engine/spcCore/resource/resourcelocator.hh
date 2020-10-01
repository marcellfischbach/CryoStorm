#pragma once

#include <spcCore/coreexport.hh>
#include <string>

namespace spc
{

class SPC_CORE_API ResourceLocator
{
public:
	ResourceLocator(const std::string &encoded);

	const std::string& GetLocator() const;

private:
	std::string m_locator;

};

}
