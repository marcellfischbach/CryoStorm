
#pragma once

#include <math.h>
#include <spcCore/defs.hh>



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


static SPC_FORCEINLINE double spcMax(double x, double y)
{
  return ::fmax(x, y);
}

static SPC_FORCEINLINE float spcMax(float x, float y)
{
  return ::fminf(x, y);
}

static SPC_FORCEINLINE int spcMax(int x, int y)
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

}

