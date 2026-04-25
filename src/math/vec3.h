#ifndef S2_VEC3_H
#define S2_VEC3_H

#define S2_VEC3_ZERO_INIT {0.0f, 0.0f, 0.0f}
#define S2_VEC3_ONE_INIT {1.0f, 1.0f, 1.0f}

#define S2_VEC3_ONE  ((vec3)S2_VEC3_ONE_INIT)
#define S2_VEC3_ZERO ((vec3)S2_VEC3_ZERO_INIT)

#define S2_VEC3_YUP       ((vec3){0.0f,  1.0f,  0.0f})
#define S2_VEC3_ZUP       ((vec3){0.0f,  0.0f,  1.0f})
#define S2_VEC3_XUP       ((vec3){1.0f,  0.0f,  0.0f})
#define S2_VEC3_FORWARD   ((vec3){0.0f,  0.0f, -1.0f})

S2_INLINE void s2_vec3_print(vec3 v)
{
    for (int i = 0; i < 3; ++i) {
        printf("%f ", v[i]);
    }
}

S2_INLINE void s2_vec3_zero(vec3 v)
{
    v[0] = v[1] = v[2] = 0.0f;
}

S2_INLINE void s2_vec3_copy(vec3 dest, vec3 src)
{
    memcpy(dest, src, sizeof(vec3));
}

S2_INLINE void s2_vec3_scale(vec3 v, float s, vec3 dest)
{
    dest[0] = v[0] * s;
    dest[1] = v[1] * s;
    dest[2] = v[2] * s;
}

S2_INLINE void s2_vec3_add(vec3 a, vec3 b, vec3 dest)
{
    dest[0] = a[0] + b[0];
    dest[1] = a[1] + b[1];
    dest[2] = a[2] + b[2];
}

S2_INLINE void s2_vec3_sub(vec3 a, vec3 b, vec3 dest)
{
    dest[0] = a[0] - b[0];
    dest[1] = a[1] - b[1];
    dest[2] = a[2] - b[2];
}

S2_INLINE void s2_vec3_mul(vec3 a, vec3 b, vec3 dest)
{
    dest[0] = a[0] * b[0];
    dest[1] = a[1] * b[1];
    dest[2] = a[2] * b[2];
}

S2_INLINE float s2_vec3_dot(vec3 a, vec3 b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

S2_INLINE float s2_vec3_norm2(vec3 v)
{
    return s2_vec3_dot(v, v);
}

S2_INLINE float s2_vec3_norm(vec3 v)
{
    return sqrtf(s2_vec3_norm2(v));
}

S2_INLINE void s2_vec3_normalize(vec3 v)
{
    float norm;

    norm = s2_vec3_norm(v);

    if (S2_UNLIKELY(norm < FLT_EPSILON)) {
        s2_vec3_zero(v);
        return;
    }

    s2_vec3_scale(v, 1.0f / norm, v);
}

S2_INLINE void s2_vec3_normalize_to(vec3 v, vec3 dest)
{
    float norm;
    norm = s2_vec3_norm(v);
    if (S2_UNLIKELY(norm < FLT_EPSILON)) {
        s2_vec3_zero(dest);
        return;
    }

    s2_vec3_scale(v, 1.0f/norm, dest);
}

S2_INLINE void s2_vec3_cross(vec3 a, vec3 b, vec3 dest)
{
    vec3 t;
    t[0] = a[1] * b[2] - a[2] * b[1];
    t[1] = a[2] * b[0] - a[0] * b[2];
    t[2] = a[0] * b[1] - a[1] * b[0];
    
    s2_vec3_copy(t, dest);
}

#endif
