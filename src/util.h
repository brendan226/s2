#ifndef S2_UTIL_H
#define S2_UTIL_H

#if defined(_MSC_VER)
    #define S2_INLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
    #define S2_INLINE static inline __attribute__((always_inline))
#else
    #define S2_INLINE static inline
#endif

#endif
