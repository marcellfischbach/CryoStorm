#pragma once

#include <ceOpenGL/openglexport.hh>
#include <ceCore/graphics/shading/iShaderAttribute.hh>
#include <GL/glew.h>

namespace cryo::opengl
{


class GL4ShaderAttributeNull : public iShaderAttribute
{
public:
	GL4ShaderAttributeNull(const std::string &name);

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
	void Bind(const Vector2f& v) override;
	void Bind(const Vector3f& v) override;
	void Bind(const Vector4f& v) override;
	void Bind(const Color4f& v) override;
	void Bind(const Matrix3f& v) override;
	void Bind(const Matrix4f& v) override;

private:
	std::string m_name;
};


}

