#pragma once

#include <ceOpenGL/openglexport.hh>
#include <ceOpenGL/gl4/shading/gl4shadertype.hh>
#include <ceCore/class.hh>
#include <ceCore/types.hh>
#include <string>
#include <stdexcept>

namespace ce::opengl
{

CS_CLASS()
class CS_OGL_API GL4Shader : public CS_SUPER(iObject)
{
  CS_CLASS_GEN_OBJECT;
public:
	GL4Shader(eGL4ShaderType type);
	virtual ~GL4Shader();

	uint32_t GetName () const;

	void SetSource(const std::string &source);

	void Compile() noexcept (false);


private:
	uint32_t m_name;

	uint32_t m_type;

	std::string m_source;

};

}