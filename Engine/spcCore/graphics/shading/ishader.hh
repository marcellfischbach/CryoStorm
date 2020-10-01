
#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>
#include <spcCore/types.hh>
#include <spcCore/graphics/renderattribute.hh>
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

	virtual UInt32 RegisterAttribute(const std::string& attributeName) = 0;

	virtual UInt32 GetAttributeId(const std::string& attributeName) = 0;

	virtual iShaderAttribute* GetShaderAttribute(UInt32 id) = 0;

	virtual iShaderAttribute* GetShaderAttribute(const std::string& attributeName) = 0;

	virtual iShaderAttribute* GetShaderAttribute(eRenderAttribute renderAttribute) = 0;

};

}