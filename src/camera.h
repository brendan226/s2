#ifndef S2_CAMERA_H
#define S2_CAMERA_H

#include "../math/math.h"

typedef struct {
    vec3 pos;

    float yaw;
    float pitch;
    float ms;
    float mouse_sens;
    float fov;

    mat4 view, model, proj;

    bool first_mouse;
    bool debug_ui;

    double last_x;
    double last_y;
} s2_camera;

void s2_camera_init(s2_camera *camera, vec3 pos, float yaw, float pitch, float fov);

#endif
