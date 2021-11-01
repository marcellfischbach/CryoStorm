#pragma once

#include <spcOpenGL/openglexport.hh>
#include <spcCore/graphics/shading/ishader.hh>
#include <spcCore/types.hh>
#include <vector>

namespace spc::opengl
{

class GL4Shader;

SPC_CLASS()
class SPC_OGL_API GL4Program : public SPC_SUPER(iShader)
{
	SPC_CLASS_GEN_OBJECT;
public:
	GL4Program();
	virtual ~GL4Program();

	void AttachShader(GL4Shader * shader);
	void DetachShader(GL4Shader * shader);

	void Link();

	uint32_t GetName() const;

	Size RegisterAttribute(const std::string & attributeName) override;

	Size GetAttributeId(const std::string & attributeName) override;

	iShaderAttribute* GetShaderAttribute(uint32_t id) override;

	iShaderAttribute* GetShaderAttribute(const std::string& attributeName) override;

	iShaderAttribute* GetShaderAttribute(eShaderAttribute shaderAttribute) override;


private:
	void RegisterRenderAttributes();

	uint32_t m_name;

	std::vector<GL4Shader*> m_shaders;

	std::vector<iShaderAttribute*> m_attributes;

};

}

