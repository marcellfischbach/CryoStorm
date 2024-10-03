#pragma  once

#include <string>
#include <ceCore/class.hh>
#include <ceCore/types.hh>
#include <ceCore/math/color4f.hh>
#include <ceCore/math/vector.hh>
#include <ceCore/math/matrix.hh>

namespace cryo
{

struct iShaderAttribute
{
  CS_DECLARE_JAVA("org/crimsonedge/core/graphics/shading/IShaderAttribute")

	virtual ~iShaderAttribute()
	{

	}

	virtual const std::string& GetName() const = 0;

	virtual bool IsValid() const = 0;

	virtual void SetArrayIndex(Size idx) = 0;

	virtual void Bind(float x) = 0;

	virtual void Bind(float x, float y) = 0;

	virtual void Bind(float x, float y, float z) = 0;

	virtual void Bind(float x, float y, float z, float w) = 0;

	virtual void Bind(int x) = 0;

	virtual void Bind(int x, int y) = 0;

	virtual void Bind(int x, int y, int z) = 0;

	virtual void Bind(int x, int y, int z, int w) = 0;

	virtual void Bind(const Vector2f& v) = 0;

	virtual void Bind(const Vector3f& v) = 0;

	virtual void Bind(const Vector4f& v) = 0;

	virtual void Bind(const Color4f& v) = 0;

	virtual void Bind(const Matrix3f& v) = 0;

	virtual void Bind(const Matrix3f* v, Size count) = 0;

	virtual void Bind(const Matrix4f& v) = 0;

	virtual void Bind(const Matrix4f* v, Size count) = 0;
};


}