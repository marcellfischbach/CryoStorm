
#pragma once

#include <math.h>
#include <spcCore/defs.hh>



namespace spc {


static SPC_FORCEINLINE double cecos(double v)
{
  return ::cos(v);
}


static SPC_FORCEINLINE float cecos(float v)
{
  return ::cosf(v);
}


static SPC_FORCEINLINE double cesin(double v)
{
  return ::sin(v);
}

static SPC_FORCEINLINE float cesin(float v)
{
  return ::sinf(v);
}

static SPC_FORCEINLINE double ceabs(double v)
{
  return ::fabs(v);
}


static SPC_FORCEINLINE float ceabs(float v)
{
  return ::fabsf(v);
}

static SPC_FORCEINLINE double cemod(double x, double y)
{
  return ::fmod(x, y);
}


static SPC_FORCEINLINE float cemod(float x, float y)
{
  return ::fmodf(x, y);
}


static SPC_FORCEINLINE double cemin(double x, double y)
{
  return ::fmin(x, y);
}

static SPC_FORCEINLINE float cemin(float x, float y)
{
  return ::fminf(x, y);
}

static SPC_FORCEINLINE int cemin(int x, int y)
{
  return x < y ? x : y;
}


static SPC_FORCEINLINE double cemax(double x, double y)
{
  return ::fmax(x, y);
}

static SPC_FORCEINLINE float cemax(float x, float y)
{
  return ::fminf(x, y);
}

static SPC_FORCEINLINE int cemax(int x, int y)
{
  return x > y ? x : y;
}

static SPC_FORCEINLINE double cesqrt(double v)
{
  return ::sqrt(v);
}

static SPC_FORCEINLINE float cesqrt(float v)
{
  return ::sqrtf(v);
}

}

