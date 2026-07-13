#ifndef S2_DISPLAY_H
#define S2_DISPLAY_H

#include <windows.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <dwmapi.h>
#include <psapi.h>

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>

typedef struct {
    VkInstance instance;
    VkDevice device;
    VkSwapchainKHR swapchain;
    VkFormat format;
    VkExtent2D extent;
} s2_display_context;

extern s2_display_context display;

void s2_load_vulkan(HWND hwnd, HINSTANCE hinstance);
void s2_register_gpu_buffer(s2_display_context *ctx, HANDLE cuda_handle);
void s2_import_gpu_memory(s2_display_context *ctx, HANDLE cuda_handle);
void s2_create_gpu_import_buffer(s2_display_context *ctx, VkDeviceSize size, VkBufferUsageFlags usage);

#endif
