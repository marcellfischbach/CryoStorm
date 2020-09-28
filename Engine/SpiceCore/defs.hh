#pragma once


#define SPICE_FLOAT_IS_ZERO(e) (e < 1e-6 && e > -1e-6)
#define SPICE_FLOAT_IS_ONE(e) (e < (1.0 + 1e-6) && e > (1.0 - 1e-6))
#define SPICE_FLOAT_IS(e, v) (e < (v + 1e-6) && e > (v - 1e-6))

#define SPICE_UNUSED(x) (void)x

#define SPICE_VERSION(maj,min,mic) ((((maj) & 0xff) << 24) | (((min) & 0xff) << 16) | ((mic) & 0xffff))

#ifdef SPICE_WIN32


#define SPICE_FORCEINLINE __forceinline

#define SPICE_ZERO(e) memset(&e, 0, sizeof(e))

#else

#define SPICE_FORCEINLINE inline

#endif
