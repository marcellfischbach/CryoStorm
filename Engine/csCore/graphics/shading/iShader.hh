
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/csTypes.hh>
#include <csCore/graphics/eShaderAttribute.hh>
#include <string>

namespace cs
{

struct iShaderAttribute;

CS_CLASS(jclass="org.cryo.core.graphics.shading.IShader")
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