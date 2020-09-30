#pragma once

#include <SpiceOpenGL/openglexport.hh>
#include <SpiceCore/graphics/shading/ishader.hh>
#include <SpiceCore/types.hh>
#include <vector>

namespace Spice
{

class GL4Shader;

SPICE_CLASS()
class SPICE_OGL_API GL4Program : public SPICE_SUPER(iShader)
{
	SPICE_CLASS_GEN_OBJECT;
public:
	GL4Program();
	virtual ~GL4Program();

	void AttachShader(GL4Shader * shader);
	void DetachShader(GL4Shader * shader);

	void Link();


	virtual UInt32 RegisterAttribute(const std::string& attributeName);

	virtual UInt32 GetAttributeId(const std::string& attributeName);

	virtual iShaderAttribute* GetShaderAttribute(UInt32 id);

	virtual iShaderAttribute* GetShaderAttribute(const std::string& attributeName);

	virtual iShaderAttribute* GetShaderAttribute(eRenderAttribute renderAttribute);


private:
	UInt32 m_name;

	std::vector<GL4Shader*> m_shaders;


};

}

