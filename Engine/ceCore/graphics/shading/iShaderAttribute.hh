#pragma  once

#include <string>
#include <ceCore/csClass.hh>
#include <ceCore/csTypes.hh>
#include <ceCore/math/csColor4f.hh>
#include <ceCore/math/csVector.hh>
#include <ceCore/math/csMatrix.hh>

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

	virtual void Bind(const csVector2f& v) = 0;

	virtual void Bind(const csVector3f& v) = 0;

	virtual void Bind(const csVector4f& v) = 0;

	virtual void Bind(const csColor4f& v) = 0;

	virtual void Bind(const csMatrix3f& v) = 0;

	virtual void Bind(const csMatrix3f* v, Size count) = 0;

	virtual void Bind(const csMatrix4f& v) = 0;

	virtual void Bind(const csMatrix4f* v, Size count) = 0;
};


}