#ifndef VEC3_H
#define VEC3_H

typedef struct {
    float x, y, z;
    float _pad;
} __attribute__((aligned(16))) vec3;



#endif
