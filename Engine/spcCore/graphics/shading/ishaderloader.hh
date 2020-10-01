#pragma once


#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>
#include <spcCore/resource/resourcelocator.hh>

namespace spc
{

struct iShader;

SPC_CLASS()
struct SPC_CORE_API iShaderLoader : public SPC_SUPER(iObject)
{
SPC_CLASS_GEN;

	virtual ~iShaderLoader()
	{

	}

	virtual iShader* Load(const ResourceLocator& locator) = 0;
};
}