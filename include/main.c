#include <stdio.h>

#include "mat4.h"
#include "vec3.h"


int main(void)
{
    mat4 m = {
        1.0f, 0.0f, 0.0f, 0.0f,
        2.0f, 0.0f, 0.0f, 0.0f,
        3.0f, 0.0f, 0.0f, 0.0f,
        4.0f, 0.0f, 0.0f, 0.0f
    };

    mat4 m2 = {
        3.0f, 3.0f, 3.0f, 3.0f,
        3.0f, 0.0f, 0.0f, 0.0f,
        3.0f, 0.0f, 0.0f, 0.0f,
        3.0f, 0.0f, 0.0f, 0.0f
    };

    mat4 res = mat4_mul(&m, &m2);
    mat4_print(&res);

    return 0;
}
