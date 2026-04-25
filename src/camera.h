#ifndef S2_CAMERA_H
#define S2_CAMERA_H

#define S2_CAMERA_MS 100.0f
#define S2_CAMERA_SENS 1.0f

#include "window.h"
#include "math/math.h"

typedef struct {
    mat4 view, projection;
    
    vec3 pos;
    vec3 target;
    vec3 up;

    float yaw;
    float pitch;
    float fov;
    float aspect;
    float ms;
    float mouse_sens;    
    float z_near;
    float z_far;   
    
    bool debug_ui;
} s2_camera;

void s2_camera_init(s2_camera *camera, s2_window *window,
                    vec3 pos, vec3 up, float aspect, float fov);
void s2_camera_set_view(s2_camera *camera, s2_window *window);
void s2_camera_update(s2_camera *camera, double last_frame,
                      double current_frame, float delta);
void s2_process_camera_input(s2_camera *camera,
                             s2_window *window, float delta_time);
void s2_set_cursor_position_callback(s2_camera *camera, s2_window *window);
void s2_enable_cursor(s2_window *window);
void s2_disable_cursor(s2_window *window);

#endif
