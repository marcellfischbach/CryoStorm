
#pragma  once

#include <spcCore/coreexport.hh>
#include <spcCore/defs.hh>
#include <spcCore/math/math.hh>
#include <spcCore/math/matrix.hh>
#include <math.h>

namespace spc
{

struct SPC_CORE_API Quaternion
{
		float x;
		float y;
		float z;
		float w;

		Quaternion(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f)
						: x(x), y(y), z(z), w(w)
		{

		}

		Quaternion(const Vector3f &v, float w = 0.0f)
						: x(v.x), y(v.y), z(v.z), w(w)
		{

		}

		SPC_FORCEINLINE Quaternion &Set(float x, float y, float z, float w)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
			return *this;
		}

		SPC_FORCEINLINE Quaternion& Invert()
		{
			w = -w;
			return *this;
		}

		SPC_FORCEINLINE Quaternion& Inverted(Quaternion &out)
		{
			return out.Set(-x, -y, -z, w);
		}


		SPC_FORCEINLINE Quaternion Inverted() const
		{
			return Quaternion(-x, -y, -z, w);
		}


		SPC_FORCEINLINE Quaternion &Normalize()
		{
			float l = sqrtf(x * x + y * y + z * z + w * w);
			x /= l;
			y /= l;
			z /= l;
			w /= l;
			return *this;
		}

		SPC_FORCEINLINE Quaternion& SetAxisAngle (float x, float y, float z, float angle)
		{
			float angle2 = angle/2.0f;
			float c = spcCos(angle2);
			float s = spcSin(angle2);
			this->x = x * s;
			this->y = y * s;
			this->z = z * s;
			this->w = c;
			return *this;
		}

		SPC_FORCEINLINE Quaternion& SetAxisAngle(const Vector3f &v, float angle)
		{
			return SetAxisAngle(v.x, v.y, v.z, angle);
		}

		SPC_FORCEINLINE static Quaternion FromAngleAxis(const Vector3f &v, float angle)
		{
			return FromAngleAxis(v.x, v.y, v.z, angle);
		}

		SPC_FORCEINLINE static Quaternion FromAngleAxis(float x, float y, float z, float angle)
		{
			float angle2 = angle/2.0f;
			float c = spcCos(angle2);
			float s = spcSin(angle2);
			return Quaternion( x * s, y * s, z * s, c);
		}



		SPC_FORCEINLINE Matrix3f &ToMatrix3(Matrix3f &out) const
		{
			float s = sqrtf(x * x + y * y + z * z + w * w);
			float s2 = 2.0f * s;
			out.Set(
							1.0f - s2 * (y * y + z * z), s2 * (x * y + z * w), s2 * (x * z - y * w),
							s2 * (x * y - z * w), 1.0f - s2 * (x * x + z * z), s2 * (y * z + x * w),
							s2 * (x * z + y * w), s2 * (y * z - x * w), 1.0f - s2 * (x * x + y * y)
			);
			return out;
		}

		SPC_FORCEINLINE Matrix4f &ToMatrix4(Matrix4f &out) const
		{
			float s = sqrtf(x * x + y * y + z * z + w * w);
			float s2 = 2.0f * s;
			return out.Set(
							1.0f - s2 * (y * y + z * z), s2 * (x * y + z * w), s2 * (x * z - y * w), 0.0f,
							s2 * (x * y - z * w), 1.0f - s2 * (x * x + z * z), s2 * (y * z + x * w), 0.0f,
							s2 * (x * z + y * w), s2 * (y * z - x * w), 1.0f - s2 * (x * x + y * y), 0.0f,
							0.0f, 0.0f, 0.0f, 1.0f
			);
		}


		SPC_FORCEINLINE friend Quaternion operator*(const Quaternion &x, const Quaternion &y)
		{
			return Quaternion(
							x.w*y.x + x.x*y.w + x.y*y.z - x.z*y.y,
							x.w*y.y - x.x*y.z + x.y*y.w + x.z*y.x,
							x.w*y.z + x.x*y.y - x.y*y.x + x.z*y.w,
							x.w*y.w - x.x*y.x - x.y*y.y - x.z*y.z
							);
		}

		SPC_FORCEINLINE Vector3f &Rotate(const Vector3f &v, Vector3f &out) const
		{
			Quaternion pn = *this * (Quaternion(v) * Inverted());
			return out.Set(pn.x, pn.y, pn.z);
		}


		SPC_FORCEINLINE Matrix3f ToMatrix3() const
		{
			Matrix3f res;
			return ToMatrix3(res);
		}


		SPC_FORCEINLINE Matrix4f ToMatrix4() const
		{
			Matrix4f res;
			return ToMatrix4(res);
		}


		void Debug()
		{
			printf ("%.2f %.2f %.2f : %.2f", x, y, z, w);
		}
};
}