#ifndef MAT4_H
#define MAT4_H

#include <math.h>
#include <emmintrin.h>

#include "types.h"

#define S2_MAT4_IDENTITY_INIT  {{1.0f, 0.0f, 0.0f, 0.0f},               \
                                 {0.0f, 1.0f, 0.0f, 0.0f},                    \
                                 {0.0f, 0.0f, 1.0f, 0.0f},                    \
                                 {0.0f, 0.0f, 0.0f, 1.0f}}

#define S2_MAT4_ZERO_INIT      {{0.0f, 0.0f, 0.0f, 0.0f},                    \
                                 {0.0f, 0.0f, 0.0f, 0.0f},                    \
                                 {0.0f, 0.0f, 0.0f, 0.0f},                    \
                                 {0.0f, 0.0f, 0.0f, 0.0f}}

#define S2_MAT4_IDENTITY ((mat4)S2_MAT4_IDENTITY_INIT)
#define S2_MAT4_ZERO     ((mat4)S2_MAT4_ZERO_INIT)

static inline void mat4_print(mat4 m)
{
    for (int i = 0; i < 4; i++) {
        printf("[ %f  %f  %f  %f ]\n", m[i][0], m[i][1], m[i][2], m[i][3]);
    }
}

static inline void mat4_identity(mat4 m)
{
    mat4 t = S2_MAT4_IDENTITY_INIT;
    memcpy(m, t, sizeof(mat4));
}

static inline void s2_mat4_mul(const mat4 a, const mat4 b, mat4 dest)
{
    __m128 a0 = _mm_load_ps(a[0]);
    __m128 a1 = _mm_load_ps(a[1]);
    __m128 a2 = _mm_load_ps(a[2]);
    __m128 a3 = _mm_load_ps(a[3]);

    for (int i = 0; i < 4; ++i) {
        __m128 bi = _mm_load_ps(b[i]);
        __m128 xxxx = _mm_shuffle_ps(bi, bi, _MM_SHUFFLE(0,0,0,0));
        __m128 yyyy = _mm_shuffle_ps(bi, bi, _MM_SHUFFLE(1,1,1,1));
        __m128 zzzz = _mm_shuffle_ps(bi, bi, _MM_SHUFFLE(2,2,2,2));
        __m128 wwww = _mm_shuffle_ps(bi, bi, _MM_SHUFFLE(3,3,3,3));
        __m128 r =
            _mm_add_ps(
                _mm_add_ps(_mm_mul_ps(a0, xxxx), _mm_mul_ps(a1, yyyy)),
                _mm_add_ps(_mm_mul_ps(a2, zzzz), _mm_mul_ps(a3, wwww)));
        _mm_store_ps(dest[i], r);
    }
}

static inline void mat4_translate(float x, float y, float z, mat4 dest)
{
    dest = S2_MAT4_IDENTITY;
    dest[3][0] = x;
    dest[3][1] = y;
    dest[3][2] = z;
}

static inline void mat4_scale(float x, float y, float z, mat4 dest)
{
    dest = S2_MAT4_IDENTITY;
    dest[0][0] = x;
    dest[1][1] = y;
    dest[2][2] = z;
}

/* static inline mat4 mat4_perspective(void) */
/* { */
    
/* } */

/* static inline Mat4 mat4_look_at(Vec3 eye, Vec3 center, Vec3 up); */

#endif

