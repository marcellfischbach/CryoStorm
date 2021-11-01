#pragma once

#include <spcOpenGL/openglexport.hh>
#include <spcOpenGL/gl4/shading/gl4shadertype.hh>
#include <spcCore/class.hh>
#include <spcCore/types.hh>
#include <string>


namespace spc::opengl
{

SPC_CLASS()
class SPC_OGL_API GL4Shader : public SPC_SUPER(iObject)
{
	SPC_CLASS_GEN_OBJECT;
public:
	GL4Shader(eGL4ShaderType type);
	virtual ~GL4Shader();

	uint32_t GetName () const;

	void SetSource(const std::string &source);

	void Compile();


private:
	uint32_t m_name;

	uint32_t m_type;

	std::string m_source;

};

}