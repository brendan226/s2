#include "camera.h"

void s2_camera_init(s2_camera *camera, s2_window *window,
                    vec3 pos, vec3 up, float aspect, float fov)
{
    memset(camera, 0, sizeof(s2_camera));
    
    s2_vec3_copy(pos, camera->pos);
    s2_vec3_copy(pos, camera->up);

    camera->yaw = -90.0f;
    camera->pitch = 0.0f;
    camera->fov = fov;
    camera->aspect = aspect;
    camera->ms = S2_CAMERA_MS;
    camera->mouse_sens = S2_CAMERA_SENS;
    camera->z_near = 0.0f;
    camera->z_far = 0.0f;
    camera->debug_ui = true;

    float rad_yaw   = camera->yaw   * (S2_PI / 180.0f);
    float rad_pitch = camera->pitch * (S2_PI / 180.0f);

    camera->target[0] = cosf(rad_yaw) * cosf(rad_pitch);
    camera->target[1] = sinf(rad_pitch);
    camera->target[2] = sinf(rad_yaw) * cosf(rad_pitch);
    
    s2_vec3_normalize_to(camera->target, camera->target);
    s2_mat4_lookat(camera->pos, camera->target, camera->up, camera->view);
    s2_mat4_perspective(camera->fov, camera->aspect,
                        camera->z_near, camera->z_far, camera->projection);
    
    s2_camera_set_view(camera, window);
}

void s2_camera_set_view(s2_camera *camera, s2_window *window)
{
    s2_perspective_rh_zo(s2_rad(camera->fov), (float)window->width/window->height, 0.1f, 100.0f, camera->projection);
}

void s2_camera_update(s2_camera *camera, double last_frame,
                      double current_frame, float delta)
{
    current_frame = glfwGetTime();
    delta = current_frame - last_frame;
    last_frame = current_frame;
}

void s2_process_camera_input(s2_camera *camera,
                             s2_window *window, float delta_time)
{
    float velocity = camera->ms * delta_time;
    vec3 movement = S2_VEC3_ZERO;
    
    switch (glfwGetKey(window->id, GLFW_KEY_W)) {
        case GLFW_PRESS:
            s2_vec3_scale(camera->target, velocity, movement);
            s2_vec3_add(camera->pos, movement, camera->pos);
            break;
        case GLFW_RELEASE:
            break;
        default:
            break;
    }

    switch (glfwGetKey(window->id, GLFW_KEY_S)) {
        case GLFW_PRESS:
            s2_vec3_scale(camera->target, velocity, movement);
            s2_vec3_sub(camera->pos, movement, camera->pos);
            break;
        case GLFW_RELEASE:            
            break;
        default:
            break;
    }

    switch (glfwGetKey(window->id, GLFW_KEY_A)) {
        case GLFW_PRESS:
            s2_vec3_scale(camera->target, velocity, movement);
            s2_vec3_sub(camera->pos, movement, camera->pos);
            break;
        case GLFW_RELEASE:            
            break;
        default:
            break;
    }

    switch (glfwGetKey(window->id, GLFW_KEY_D)) {
        case GLFW_PRESS:
            s2_vec3_scale(camera->target, velocity, movement);
            s2_vec3_add(camera->pos, movement, camera->pos);
            break;
        case GLFW_RELEASE:            
            break;
        default:
            break;
    }
}

void s2_cursor_position_callback(s2_window *window)
{
    s2_camera *camera = (s2_camera*)glfwGetWindowUserPointer(window->id);
    /* ImGuiIO& io = ImGui::GetIO(); */
}

void s2_set_cursor_position_callback(s2_camera *camera, s2_window *window)
{
    glfwSetWindowUserPointer(window->id, camera);
}

void s2_enable_cursor(s2_window *window)
{
    glfwSetInputMode(window->id, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void s2_disable_cursor(s2_window *window)
{
    glfwSetInputMode(window->id, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
