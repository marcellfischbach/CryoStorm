#pragma once

#include <spcOpenGL/openglexport.hh>
#include <spcOpenGL/gl4/shading/gl4shadertype.hh>
#include <spcCore/class.hh>
#include <spcCore/types.hh>
#include <string>


namespace spc
{

SPC_CLASS()
class SPC_OGL_API GL4Shader : public SPC_SUPER(iObject)
{
	SPC_CLASS_GEN_OBJECT;
public:
	GL4Shader(eGL4ShaderType type);
	virtual ~GL4Shader();

	UInt32 GetName () const;

	void SetSource(const std::string &source);

	void Compile();


private:
	UInt32 m_name;

	UInt32 m_type;

	std::string m_source;

};

}