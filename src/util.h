#ifndef S2_UTIL_H
#define S2_UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define S2_SUCCESS 0
#define S2_ERROR_INVALID_VALUE = 1
#define S2_ERROR_INVALID_DEVICE = 101
#define S2_ERROR_SYSTEM_DRIVER_MISMATCH = 803
#define S2_ERROR_COMPAT_NOT_SUPPORTED_ON_DEVICE = 804

#if defined(_MSC_VER)
    #define S2_INLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
    #define S2_INLINE static inline __attribute__((always_inline))
#else
    #define S2_INLINE static inline
#endif

#endif
