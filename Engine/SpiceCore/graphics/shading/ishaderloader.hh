#pragma once


#include <SpiceCore/coreexport.hh>
#include <SpiceCore/class.hh>
#include <SpiceCore/resource/resourcelocator.hh>

namespace Spice
{

struct iShader;

SPICE_CLASS()
struct SPICE_CORE_API iShaderLoader : public SPICE_SUPER(iObject)
{
SPICE_CLASS_GEN;

	virtual ~iShaderLoader()
	{

	}

	virtual iShader* Load(const ResourceLocator& locator) = 0;
};
}