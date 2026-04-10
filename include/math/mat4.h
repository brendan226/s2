#ifndef S2_MAT4_H
#define S2_MAT4_H

#include "types.h"

#define S2_MAT4_IDENTITY_INIT  {{1.0f, 0.0f, 0.0f, 0.0f},    \
                                {0.0f, 1.0f, 0.0f, 0.0f},    \
                                {0.0f, 0.0f, 1.0f, 0.0f},    \
                                {0.0f, 0.0f, 0.0f, 1.0f}}

#define S2_MAT4_ZERO_INIT      {{0.0f, 0.0f, 0.0f, 0.0f},    \
                                {0.0f, 0.0f, 0.0f, 0.0f},    \
                                {0.0f, 0.0f, 0.0f, 0.0f},    \
                                {0.0f, 0.0f, 0.0f, 0.0f}}

#define S2_MAT4_IDENTITY ((mat4)S2_MAT4_IDENTITY_INIT)
#define S2_MAT4_ZERO     ((mat4)S2_MAT4_ZERO_INIT)

S2_INLINE void s2_mat4_print(mat4 m)
{
    for (int i = 0; i < 4; i++) {
        printf("[ %f  %f  %f  %f ]\n",
        m[i][0], m[i][1], m[i][2], m[i][3]);
    }
}

S2_INLINE void s2_mat4_identity(mat4 dest)
{
    mat4 t = S2_MAT4_IDENTITY;
    memcpy(dest, t, sizeof(mat4));
}

S2_INLINE void s2_mat4_mul(const mat4 a, const mat4 b, mat4 dest)
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

S2_INLINE void s2_mat4_pick3(mat4 m, mat4 dest)
{

}

S2_INLINE void s2_mat4_trace(mat4 m)
{

}

S2_INLINE void s2_mat4_transpose(mat4 m)
{
    
}

S2_INLINE void s2_mat4_transpose_to(mat4 m, mat4 dest)
{
    
}

S2_INLINE void s2_mat4_scale(mat4 m, float s)
{

}

S2_INLINE void s2_mat4_perspective(float fov, float aspect, float near, float far, mat4 dest)
{
    
}

S2_INLINE void s2_mat4_lookat(vec3 eye, vec3 center, vec3 up, mat4 dest)
{

}

S2_INLINE void s2_mat4_translate(mat4 m, vec3 v)
{
    
}

S2_INLINE void s2_mat4_rotate(mat4 m, float angle, vec3 axis)
{
    
}

#endif

