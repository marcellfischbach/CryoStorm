
#pragma once

#include <math.h>
#include <spcCore/defs.hh>
#include <spcCore/types.hh>



namespace spc {


static SPC_FORCEINLINE double spcCos(double v)
{
  return ::cos(v);
}


static SPC_FORCEINLINE float spcCos(float v)
{
  return ::cosf(v);
}


static SPC_FORCEINLINE double spcSin(double v)
{
  return ::sin(v);
}

static SPC_FORCEINLINE float spcSin(float v)
{
  return ::sinf(v);
}


static SPC_FORCEINLINE double spcTan(double v)
{
  return ::tan(v);
}

static SPC_FORCEINLINE float spcTan(float v)
{
  return ::tanf(v);
}

static SPC_FORCEINLINE double spcAbs(double v)
{
  return ::fabs(v);
}


static SPC_FORCEINLINE float spcAbs(float v)
{
  return ::fabsf(v);
}

static SPC_FORCEINLINE double spcMod(double x, double y)
{
  return ::fmod(x, y);
}


static SPC_FORCEINLINE float spcMod(float x, float y)
{
  return ::fmodf(x, y);
}


static SPC_FORCEINLINE double spcMin(double x, double y)
{
  return ::fmin(x, y);
}

static SPC_FORCEINLINE float spcMin(float x, float y)
{
  return ::fminf(x, y);
}

static SPC_FORCEINLINE int spcMin(int x, int y)
{
  return x < y ? x : y;
}

static SPC_FORCEINLINE Size spcMin(Size x, Size y)
{
  return x < y ? x : y;
}


static SPC_FORCEINLINE double spcMax(double x, double y)
{
  return ::fmax(x, y);
}

static SPC_FORCEINLINE float spcMax(float x, float y)
{
  return ::fmaxf(x, y);
}

static SPC_FORCEINLINE int spcMax(int x, int y)
{
  return x > y ? x : y;
}

static SPC_FORCEINLINE Size spcMax(Size x, Size y)
{
  return x > y ? x : y;
}


static SPC_FORCEINLINE double spcSqrt(double v)
{
  return ::sqrt(v);
}

static SPC_FORCEINLINE float spcSqrt(float v)
{
  return ::sqrtf(v);
}

static SPC_FORCEINLINE float spcRad2Deg(float rad)
{
  return rad * 180.0f / (float)M_PI;
}


static SPC_FORCEINLINE float spcDeg2Rad(float deg)
{
  return deg * (float)M_PI / 180.0f;
}


static SPC_FORCEINLINE float spcClamp01(float v)
{
  return v < 0.0f ? 0.0f : (v > 1.0f ? 1.0f : v);
}

static SPC_FORCEINLINE double spcClamp01(double v)
{
  return v < 0.0 ? 0.0 : (v > 1.0 ? 1.0 : v);
}

static SPC_FORCEINLINE float spcClamp(float v, float min, float max)
{
  return v < min ? min : (v > max ? max : v);
}

static SPC_FORCEINLINE double spcClamp(double v, double min, double max)
{
  return v < min ? min : (v > max ? max : v);
}


template <typename T>
static SPC_FORCEINLINE T spcClamp(T v, T min, T max)
{
  return v < min ? min : (v > max ? max : v);

}

}

