#include <stdio.h>
#include <stdlib.h>

#include "window.h"

void s2_window_init(s2_window *window)
{
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW.\n");
        exit(EXIT_FAILURE);
    }
    
    window->monitor = glfwGetPrimaryMonitor();
    window->mode = glfwGetVideoMode(window->monitor);
    
    window->width = window->mode->width;
    window->height = window->mode->height;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window->id = glfwCreateWindow(window->mode->width,                                  window->mode->height, "s2v-0.0.1", NULL, NULL);

    if (!window->id) {
        fprintf(stderr, "Failed to create window.\n");
        exit(EXIT_FAILURE);
    }
}

void s2_window_fullscreen(s2_window *window)
{
    glfwSetWindowMonitor(window->id, window->monitor, 0, 0, window->mode->width,
                         window->mode->height, window->mode->refreshRate);
}

void s2_window_clean(s2_window *window)
{
    window->monitor = NULL;
    window->mode = NULL;
    window->width = 0;
    window->height = 0;
    glfwDestroyWindow(window->id);
}
