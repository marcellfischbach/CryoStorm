#pragma once

#include <SpiceOpenGL/openglexport.hh>
#include <SpiceOpenGL/gl4/shading/gl4shadertype.hh>
#include <SpiceCore/class.hh>
#include <SpiceCore/types.hh>
#include <string>


namespace Spice
{

SPICE_CLASS()
class SPICE_OGL_API GL4Shader : public SPICE_SUPER(iObject)
{
	SPICE_CLASS_GEN_OBJECT;
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