#pragma once

#include <ceOpenGL/csOpenGLExport.hh>
#include <ceOpenGL/gl4/shading/eGL4ShaderType.hh>
#include <ceCore/csClass.hh>
#include <ceCore/csTypes.hh>
#include <string>
#include <stdexcept>

namespace cryo::opengl
{

CS_CLASS()
class CS_OGL_API csGL4Shader : public CS_SUPER(iObject)
{
  CS_CLASS_GEN_OBJECT;
public:
	csGL4Shader(eGL4ShaderType type);
	virtual ~csGL4Shader();

	uint32_t GetName () const;

	void SetSource(const std::string &source);

	void Compile() noexcept (false);


private:
	uint32_t m_name;

	uint32_t m_type;

	std::string m_source;

};

}