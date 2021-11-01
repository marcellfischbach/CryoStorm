
#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>
#include <spcCore/types.hh>
#include <spcCore/graphics/eshaderattribute.hh>
#include <string>

namespace spc
{

struct iShaderAttribute;

SPC_CLASS()

struct SPC_CORE_API iShader : public SPC_SUPER(iObject)
{
SPC_CLASS_GEN;

	virtual ~iShader()
	{
	}

	virtual Size RegisterAttribute(const std::string& attributeName) = 0;

	virtual Size GetAttributeId(const std::string& attributeName) = 0;

	virtual iShaderAttribute* GetShaderAttribute(uint32_t id) = 0;

	virtual iShaderAttribute* GetShaderAttribute(const std::string& attributeName) = 0;

	virtual iShaderAttribute* GetShaderAttribute(eShaderAttribute shaderAttribute) = 0;

};

}