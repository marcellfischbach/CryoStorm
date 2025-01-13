#pragma once

#include <csOpenGL/csOpenGLExport.hh>
#include <csOpenGL/gl4/shading/eGL4ShaderType.hh>
#include <csCore/csClass.hh>
#include <csCore/csTypes.hh>
#include <csCore/resource/iAsset.hh>
#include <string>
#include <stdexcept>

namespace cs::opengl
{

CS_CLASS()
class CS_OGL_API csGL4Shader : public CS_SUPER(iAsset)
{
  CS_CLASS_GEN_OBJECT;
  CS_ASSET_GEN;
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