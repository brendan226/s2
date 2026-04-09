#ifndef TYPES_H
#define TYPES_H

typedef int ivec2[2];
typedef int ivec3[3];
typedef int ivec4[4];

typedef float vec2[2];
typedef float vec3[3];
typedef __attribute__((aligned(16))) float vec4[4];

typedef __attribute__((aligned(16))) vec2 mat2[2];
typedef vec3 mat3[3];
typedef __attribute__((aligned(16))) vec4 mat4[4];

#endif
