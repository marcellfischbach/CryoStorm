#pragma once

#include <csOpenGL/csOpenGLExport.hh>
#include <csCore/graphics/shading/iShaderAttribute.hh>
#include <GL/glew.h>

namespace cs::opengl
{


class csGL4ShaderAttribute : public iShaderAttribute
{
public:
	csGL4ShaderAttribute(GLint location, const std::string &name);

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
	void Bind(const csMatrix3f* v, Size count) override;
	void Bind(const csMatrix4f& v) override;
	void Bind(const csMatrix4f* v, Size count) override;
  void Bind (eTextureUnit unit);

private:
	GLint m_location;
	GLint m_arrayIndex;
	GLint m_absLocation;
	std::string m_name;
};


}

