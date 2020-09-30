
#pragma once

#include <SpiceCore/coreexport.hh>
#include <SpiceCore/class.hh>
#include <SpiceCore/types.hh>
#include <SpiceCore/graphics/renderattribute.hh>
#include <string>

namespace Spice
{

struct iShaderAttribute;

SPICE_CLASS()

struct SPICE_CORE_API iShader : public SPICE_SUPER(iObject)
{
SPICE_CLASS_GEN;

	virtual ~iShader()
	{
	}

	virtual UInt32 RegisterAttribute(const std::string& attributeName) = 0;

	virtual UInt32 GetAttributeId(const std::string& attributeName) = 0;

	virtual iShaderAttribute* GetShaderAttribute(UInt32 id) = 0;

	virtual iShaderAttribute* GetShaderAttribute(const std::string& attributeName) = 0;

	virtual iShaderAttribute* GetShaderAttribute(eRenderAttribute renderAttribute) = 0;

};

}