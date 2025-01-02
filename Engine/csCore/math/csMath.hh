
#pragma once

#include <cmath>
#include <cfloat>
#include <csCore/csDefs.hh>
#include <csCore/csTypes.hh>



#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

namespace cs {


static CS_FORCEINLINE double ceCos(double v)
{
  return ::cos(v);
}


static CS_FORCEINLINE float ceCos(float v)
{
  return ::cosf(v);
}


static CS_FORCEINLINE double ceSin(double v)
{
  return ::sin(v);
}

static CS_FORCEINLINE float ceSin(float v)
{
  return ::sinf(v);
}


static CS_FORCEINLINE double ceTan(double v)
{
  return ::tan(v);
}

static CS_FORCEINLINE float ceTan(float v)
{
  return ::tanf(v);
}

static CS_FORCEINLINE double ceAbs(double v)
{
  return ::fabs(v);
}


static CS_FORCEINLINE float ceAbs(float v)
{
  return ::fabsf(v);
}

static CS_FORCEINLINE double ceMod(double x, double y)
{
  return ::fmod(x, y);
}


static CS_FORCEINLINE float ceMod(float x, float y)
{
  return ::fmodf(x, y);
}

template<typename T>
static CS_FORCEINLINE T ceMin(const T &x, const T &y)
{
  bool v = x < y;
  return v * x + !v * y;
}


template<typename T>
static CS_FORCEINLINE T ceMax( const T &x, const T &y)
{
  bool v = x > y;
  return v * x + !v * y;
}


static CS_FORCEINLINE double ceSqrt(double v)
{
  return ::sqrt(v);
}

static CS_FORCEINLINE float ceSqrt(float v)
{
  return ::sqrtf(v);
}

static CS_FORCEINLINE float ceRad2Deg(float rad)
{
  return rad * 180.0f / (float)M_PI;
}


static CS_FORCEINLINE float ceDeg2Rad(float deg)
{
  return deg * (float)M_PI / 180.0f;
}


static CS_FORCEINLINE float ceClamp01(float v)
{
  return v < 0.0f ? 0.0f : (v > 1.0f ? 1.0f : v);
}

static CS_FORCEINLINE double ceClamp01(double v)
{
  return v < 0.0 ? 0.0 : (v > 1.0 ? 1.0 : v);
}

static CS_FORCEINLINE float ceClamp(float v, float min, float max)
{
  return v < min ? min : (v > max ? max : v);
}

static CS_FORCEINLINE double ceClamp(double v, double min, double max)
{
  return v < min ? min : (v > max ? max : v);
}


template <typename T>
static CS_FORCEINLINE T ceClamp(T v, T min, T max)
{
  return v < min ? min : (v > max ? max : v);

}

}

