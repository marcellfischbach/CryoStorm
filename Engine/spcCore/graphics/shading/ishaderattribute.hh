#pragma  once

#include <string>
#include <spcCore/types.hh>
#include <spcCore/math/color4f.hh>
#include <spcCore/math/vector.hh>
#include <spcCore/math/matrix.hh>

namespace spc
{

struct iShaderAttribute
{
	virtual ~iShaderAttribute()
	{

	}

	virtual const std::string& GetName() const = 0;

	virtual bool IsValid() const = 0;

	virtual void SetArrayIndex(UInt16 idx) = 0;

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

	virtual void Bind(const Matrix4f& v) = 0;

};


}