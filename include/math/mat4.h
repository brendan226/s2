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

S2_INLINE void s2_mat4_perspective(float fov, float aspect, float z_near, float z_far, mat4 dest)
{    
    float f = 1.0f / tanf(fov * 0.5f);
    
    memset(dest, 0, sizeof(mat4));
    
    dest[0][0] =  f / aspect;
    dest[1][1] = -f;
    dest[2][2] =  z_far / (z_near - z_far);
    dest[2][3] = -1.0f;
    dest[3][2] = (z_near * z_far) / (z_near - z_far);
}

S2_INLINE void s2_mat4_lookat(vec3 eye, vec3 center, vec3 up, mat4 dest)
{
    // forward vector
    float fx = center[0] - eye[0];
    float fy = center[1] - eye[1];
    float fz = center[2] - eye[2];
    float flen = 1.0f / sqrtf(fx*fx + fy*fy + fz*fz);
    fx *= flen; fy *= flen; fz *= flen;

    // right vector = forward x up
    float rx = fy*up[2] - fz*up[1];
    float ry = fz*up[0] - fx*up[2];
    float rz = fx*up[1] - fy*up[0];
    float rlen = 1.0f / sqrtf(rx*rx + ry*ry + rz*rz);
    rx *= rlen; ry *= rlen; rz *= rlen;

    // up vector = right x forward
    float ux = ry*fz - rz*fy;
    float uy = rz*fx - rx*fz;
    float uz = rx*fy - ry*fx;

    dest[0][0] =  rx;
    dest[0][1] =  ux;
    dest[0][2] = -fx;
    dest[0][3] =  0.0f;

    dest[1][0] =  ry;
    dest[1][1] =  uy;
    dest[1][2] = -fy;
    dest[1][3] =  0.0f;

    dest[2][0] =  rz;
    dest[2][1] =  uz;
    dest[2][2] = -fz;
    dest[2][3] =  0.0f;

    dest[3][0] = -(rx*eye[0] + ry*eye[1] + rz*eye[2]);
    dest[3][1] = -(ux*eye[0] + uy*eye[1] + uz*eye[2]);
    dest[3][2] =  (fx*eye[0] + fy*eye[1] + fz*eye[2]);
    dest[3][3] =  1.0f;
}

S2_INLINE void s2_mat4_translate(mat4 m, vec3 v)
{
    
}

S2_INLINE void s2_mat4_rotate(mat4 m, float angle, vec3 axis)
{
    
}

#endif

