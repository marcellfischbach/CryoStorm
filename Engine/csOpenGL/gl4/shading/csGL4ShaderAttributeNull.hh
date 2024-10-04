#pragma once

#include <csOpenGL/csOpenGLExport.hh>
#include <csCore/graphics/shading/iShaderAttribute.hh>
#include <GL/glew.h>

namespace cryo::opengl
{


class csGL4ShaderAttributeNull : public iShaderAttribute
{
public:
	csGL4ShaderAttributeNull(const std::string &name);

	const std::string& GetName() const override;

	bool IsValid() const override;

	void SetArrayIndex(Size idx) override;
	void Bind(float x) override;
	void Bind(float x, float y) override;
	void Bind(float x, float y, float z) override;
	void Bind(float x, float y, float z, float w) override;
	void Bind(int x) override;
	void Bind(int x, int y) override;
	void Bind(int x, int y, int z) override;
	void Bind(int x, int y, int z, int w) override;
	void Bind(const csVector2f& v) override;
	void Bind(const csVector3f& v) override;
	void Bind(const csVector4f& v) override;
	void Bind(const csColor4f& v) override;
	void Bind(const csMatrix3f& v) override;
	void Bind(const csMatrix4f& v) override;

private:
	std::string m_name;
};


}

