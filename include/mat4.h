#ifndef MAT4_H
#define MAT4_H

#define SIZE 4

#include <math.h>
#include <emmintrin.h>

typedef struct {
    float elements[SIZE][SIZE];
} __attribute__((aligned(16))) mat4;

static inline void mat4_print(mat4 *m)
{
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            printf("[%f]", m->elements[j][i]);
        }
        printf("\n");
    }
}

static inline mat4 mat4_identity(void)
{
    mat4 m = {0};
    m.elements[0][0] = 1.0f;
    m.elements[1][1] = 1.0f;
    m.elements[2][2] = 1.0f;
    m.elements[3][3] = 1.0f;    
    return m;
}

/* **What each intrinsic does:** */
/* - `_mm_load_ps` — loads 4 floats from aligned memory into a 128 bit register */
/* - `_mm_set1_ps` — broadcasts one float into all 4 lanes `[x,x,x,x]` */
/* - `_mm_mul_ps` — multiplies 4 floats simultaneously */
/* - `_mm_add_ps` — adds 4 floats simultaneously */
/* - `_mm_store_ps` — stores 4 floats from register back to aligned memory */

static inline mat4 mat4_mul(mat4 *a, mat4 *b)
{
    mat4 res;  
    __m128 a0 = _mm_load_ps(a->elements[0]);
    __m128 a1 = _mm_load_ps(a->elements[1]);
    __m128 a2 = _mm_load_ps(a->elements[2]);
    __m128 a3 = _mm_load_ps(a->elements[3]);

    for (int i = 0; i < 4; ++i) {
        __m128 b0 = _mm_set1_ps(b->elements[i][0]);
        __m128 b1 = _mm_set1_ps(b->elements[i][1]);
        __m128 b2 = _mm_set1_ps(b->elements[i][2]);
        __m128 b3 = _mm_set1_ps(b->elements[i][3]);

        __m128 col = _mm_add_ps(
                                _mm_add_ps(_mm_mul_ps(a0, b0), _mm_mul_ps(a1, b1)),
                                _mm_add_ps(_mm_mul_ps(a2, b2), _mm_mul_ps(a3, b3))
                                );

        _mm_store_ps(res.elements[i], col);
    }

    return res;
}

static inline mat4 mat4_translate(float x, float y, float z)
{
    mat4 m = mat4_identity();    
    m.elements[3][0] = x;
    m.elements[3][1] = y;
    m.elements[3][2] = z;
    return m;
}

static inline mat4 mat4_scale(float x, float y, float z)
{
    mat4 m = mat4_identity();   
    m.elements[0][0] = x;
    m.elements[1][1] = y;
    m.elements[2][2] = z;
    return m;
}

static inline mat4 mat4_rotate_x(float angle_rad)
{
    float s, c;
    sincosf(angle_rad, &s, &c);
    mat4 m = mat4_identity();
    m.elements[1][1] = c;
    m.elements[1][2] = s;
    m.elements[2][1] = -s;
    m.elements[2][2] = c;
    return m;
}

static inline mat4 mat4_rotate_y(float angle_rad)
{
    float s, c;
    sincosf(angle_rad, &s, &c);
    mat4 m = mat4_identity();
    m.elements[0][0] = c;
    m.elements[0][2] = -s;
    m.elements[2][0] = s;
    m.elements[2][2] = c;
    return m;
        
}

static inline mat4 mat4_rotate_z(float angle_rad)
{
    float s, c;
    sincosf(angle_rad, &s, &c);
    mat4 m = mat4_identity();    
    m.elements[0][0] =  c;
    m.elements[0][1] =  s;
    m.elements[1][0] = -s;
    m.elements[1][1] =  c;    
    return m;
}

static inline mat4 mat4_perspective(void)
{
    
}

/* static inline Mat4 mat4_look_at(Vec3 eye, Vec3 center, Vec3 up); */

#endif
