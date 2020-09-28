
#pragma once

#include <math.h>
#include <cecore/cedefs.hh>


static CE_FORCEINLINE double cecos(double v)
{
  return ::cos(v);
}


static CE_FORCEINLINE float cecos(float v)
{
  return ::cosf(v);
}


static CE_FORCEINLINE double cesin(double v)
{
  return ::sin(v);
}

static CE_FORCEINLINE float cesin(float v)
{
  return ::sinf(v);
}

static CE_FORCEINLINE double ceabs(double v)
{
  return ::abs(v);
}


static CE_FORCEINLINE float ceabs(float v)
{
  return ::fabsf(v);
}

static CE_FORCEINLINE double cemod(double x, double y)
{
  return ::fmod(x, y);
}


static CE_FORCEINLINE float cemod(float x, float y)
{
  return ::fmodf(x, y);
}


static CE_FORCEINLINE double cemin(double x, double y)
{
  return ::fmin(x, y);
}

static CE_FORCEINLINE float cemin(float x, float y)
{
  return ::fminf(x, y);
}

static CE_FORCEINLINE int cemin(int x, int y)
{
  return x < y ? x : y;
}


static CE_FORCEINLINE double cemax(double x, double y)
{
  return ::fmax(x, y);
}

static CE_FORCEINLINE float cemax(float x, float y)
{
  return ::fminf(x, y);
}

static CE_FORCEINLINE int cemax(int x, int y)
{
  return x > y ? x : y;
}

static CE_FORCEINLINE double cesqrt(double v)
{
  return ::sqrt(v);
}

static CE_FORCEINLINE float cesqrt(float v)
{
  return ::sqrtf(v);
}

