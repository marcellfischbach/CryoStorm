#pragma once


#define CE_FLOAT_IS_ZERO(e) (e < 1e-6f && e > -1e-6f)
#define CE_FLOAT_IS_ONE(e) (e < (1.0f + 1e-6f) && e > (1.0f - 1e-6f))
#define CE_FLOAT_IS(e, v) (e < (v + 1e-6f) && e > (v - 1e-6f))



#define CE_UNUSED(x) (void)x

#define CE_VERSION(maj,min,mic) ((((maj) & 0xff) << 24) | (((min) & 0xff) << 16) | ((mic) & 0xffff))

#ifdef CE_WIN32

#define CE_NODISCARD [[nodiscard]]

#define CE_MAYBE_UNUSED [[maybe_unused]]

#define CE_FORCEINLINE __forceinline

#define CE_ZERO(e) memset(&e, 0, sizeof(e))

#else

#define CE_NODISCARD

#define CE_UNUSED(x) (void)x

#define CE_FORCEINLINE inline
#define CE_ZERO(e) memset(&e, 0, sizeof(e))

#endif
