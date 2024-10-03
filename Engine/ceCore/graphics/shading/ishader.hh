
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/types.hh>
#include <ceCore/graphics/eshaderattribute.hh>
#include <string>

namespace cryo
{

struct iShaderAttribute;

CS_CLASS(jclass="org.crimsonedge.core.graphics.shading.IShader")
struct CS_CORE_API iShader : public CS_SUPER(iObject)
{
CS_CLASS_GEN;

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