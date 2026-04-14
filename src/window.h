#ifndef S2_WINDOW_H
#define S2_WINDOW_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

typedef struct {
    GLFWmonitor* monitor;
    const GLFWvidmode* mode;
    GLFWwindow *id;
} s2_window;

void s2_window_init(s2_window *window)
{
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW.\n");
        exit(EXIT_FAILURE);
    }
    
    window->monitor = glfwGetPrimaryMonitor();
    window->mode = glfwGetVideoMode(window->monitor);

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window->id = glfwCreateWindow(window->mode->width, window->mode->height, "s2v-0.0.1", NULL, NULL);

    glfwSetWindowMonitor(window->id, window->monitor, 0, 0,
                         window->mode->width, window->mode->height, window->mode->refreshRate);
    if (!window->id) {
        fprintf(stderr, "Failed to create window.\n");
        exit(EXIT_FAILURE);
    }
}

#endif
