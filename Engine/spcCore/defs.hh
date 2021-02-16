#pragma once


#define SPC_FLOAT_IS_ZERO(e) (e < 1e-6f && e > -1e-6f)
#define SPC_FLOAT_IS_ONE(e) (e < (1.0f + 1e-6f) && e > (1.0f - 1e-6f))
#define SPC_FLOAT_IS(e, v) (e < (v + 1e-6f) && e > (v - 1e-6f))

#define SPC_NODISCARD [[nodiscard]]

#define SPC_UNUSED(x) (void)x

#define SPC_VERSION(maj,min,mic) ((((maj) & 0xff) << 24) | (((min) & 0xff) << 16) | ((mic) & 0xffff))

#ifdef SPC_WIN32


#define SPC_FORCEINLINE __forceinline

#define SPC_ZERO(e) memset(&e, 0, sizeof(e))

#else

#define SPC_FORCEINLINE inline

#endif
