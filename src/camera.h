#ifndef S2_CAMERA_H
#define S2_CAMERA_H

#include "../math/math.h"

typedef struct {
    mat4 view, projection;
    
    vec3 position;
    vec3 target;
    vec3 up;

    float yaw;
    float pitch;
    float fov;
    float aspect;
    
    float z_near;
    float z_far;   
    
    bool debug_ui;
} s2_camera;

void s2_camera_init(s2_camera *camera, vec3 pos, vec3 up, float fov);
void s2_camera_update(s2_camera *camera, GLFWwindow *window);
void s2_camera_process_input(s2_camera *camera, GLFWwindow *window, float dt);
void s2_camera_process_mouse(s2_camera *camera, double x_offset, double y_offset);

#endif
