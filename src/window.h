#ifndef S2_WINDOW_H
#define S2_WINDOW_H

#include <GLFW/glfw3.h>

typedef struct {
    GLFWmonitor* monitor;
    const GLFWvidmode* mode;
    GLFWwindow *id;
    
    unsigned int width;
    unsigned int height;
} s2_window;

void s2_window_init(s2_window *window);
void s2_window_fullscreen(s2_window *window);
void s2_window_clean(s2_window *window);

#endif
