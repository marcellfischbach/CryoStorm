#pragma once

#include <cstring>

#define CS_FLOAT_IS_ZERO(e) (e < 1e-6f && e > -1e-6f)
#define CS_FLOAT_IS_ONE(e) (e < (1.0f + 1e-6f) && e > (1.0f - 1e-6f))
#define CS_FLOAT_IS(e, v) (e < (v + 1e-6f) && e > (v - 1e-6f))



#define CS_UNUSED(x) (void)x

#define CS_VERSION(maj,min,mic) ((((maj) & 0xff) << 24) | (((min) & 0xff) << 16) | ((mic) & 0xffff))

#if defined (_MSC_VER)


#define CS_NODISCARD [[nodiscard]]
#define CS_MAYBE_UNUSED [[maybe_unused]]
#define CS_FORCEINLINE __forceinline
#define CS_ZERO(e) memset(&e, 0, sizeof(e))

#elif defined(__GNUC__)


#define CS_NODISCARD
#define CS_MAYBE_UNUSED
#define CS_FORCEINLINE inline
#define CS_ZERO(e) memset(&e, 0, sizeof(e))

#else

#define CS_NODISCARD
#define CS_MAYBE_UNUSED
#define CS_FORCEINLINE inline
#define CS_ZERO(e) memset(&e, 0, sizeof(e))

#endif
