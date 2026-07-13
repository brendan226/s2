#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <windows.h>
#include <dwmapi.h>
#include <psapi.h>

#include "display.h"

static bool running = true;
static HMODULE vklib = NULL;
static s2_display_context display = {0};

PFN_vkCreateInstance s2_vkCreateInstance = NULL;
PFN_vkGetInstanceProcAddr s2_vkGetInstanceProcAddr = NULL;
PFN_vkCreateWin32SurfaceKHR s2_vkCreateWin32SurfaceKHR = NULL;
PFN_vkEnumeratePhysicalDevices s2_vkEnumeratePhysicalDevices = NULL;
PFN_vkGetPhysicalDeviceQueueFamilyProperties s2_vkGetPhysicalDeviceQueueFamilyProperties = NULL;
PFN_vkGetPhysicalDeviceSurfaceSupportKHR s2_vkGetPhysicalDeviceSurfaceSupportKHR = NULL;
PFN_vkCreateDevice s2_vkCreateDevice = NULL;
PFN_vkGetPhysicalDeviceFormatProperties s2_vkGetPhysicalDeviceFormatProperties = NULL;
PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR s2_vkGetPhysicalDeviceSurfaceCapabilitiesKHR = NULL;
PFN_vkGetDeviceQueue s2_vkGetDeviceQueue = NULL;
PFN_vkCreateSwapchainKHR s2_vkCreateSwapchainKHR = NULL;
PFN_vkGetSwapchainImagesKHR s2_vkGetSwapchainImagesKHR = NULL;
PFN_vkCreateImageView s2_vkCreateImageView = NULL;
// cut
PFN_vkCreateCommandPool     s2_vkCreateCommandPool     = NULL;
PFN_vkAllocateCommandBuffers s2_vkAllocateCommandBuffers = NULL;
PFN_vkBeginCommandBuffer    s2_vkBeginCommandBuffer    = NULL;
PFN_vkCmdPipelineBarrier    s2_vkCmdPipelineBarrier    = NULL;
PFN_vkCmdClearColorImage    s2_vkCmdClearColorImage    = NULL;
PFN_vkEndCommandBuffer      s2_vkEndCommandBuffer      = NULL;
PFN_vkCreateSemaphore       s2_vkCreateSemaphore       = NULL;
PFN_vkCreateFence           s2_vkCreateFence           = NULL;
PFN_vkAcquireNextImageKHR   s2_vkAcquireNextImageKHR   = NULL;
PFN_vkQueueSubmit           s2_vkQueueSubmit           = NULL;
PFN_vkQueuePresentKHR       s2_vkQueuePresentKHR       = NULL;
PFN_vkWaitForFences         s2_vkWaitForFences         = NULL;
PFN_vkResetFences           s2_vkResetFences           = NULL;
PFN_vkResetCommandBuffer    s2_vkResetCommandBuffer    = NULL;

static void s2_load_vulkan(HWND hwnd, HINSTANCE hinstance)
{
    vklib = LoadLibraryA("vulkan-1.dll");
    if (!vklib) return;

    s2_vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)
        GetProcAddress(vklib, "vkGetInstanceProcAddr");

    s2_vkCreateInstance = (PFN_vkCreateInstance)
        s2_vkGetInstanceProcAddr(VK_NULL_HANDLE, "vkCreateInstance");

    s2_create_instance(&display.instance);

    s2_vkCreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)
        s2_vkGetInstanceProcAddr(display.instance, "vkCreateWin32SurfaceKHR");
    s2_vkEnumeratePhysicalDevices = (PFN_vkEnumeratePhysicalDevices)
        s2_vkGetInstanceProcAddr(display.instance, "vkEnumeratePhysicalDevices");
    s2_vkGetPhysicalDeviceQueueFamilyProperties = (PFN_vkGetPhysicalDeviceQueueFamilyProperties)
        s2_vkGetInstanceProcAddr(display.instance, "vkGetPhysicalDeviceQueueFamilyProperties");
    s2_vkGetPhysicalDeviceSurfaceSupportKHR = (PFN_vkGetPhysicalDeviceSurfaceSupportKHR)
        s2_vkGetInstanceProcAddr(display.instance, "vkGetPhysicalDeviceSurfaceSupportKHR");
    s2_vkCreateDevice = (PFN_vkCreateDevice)
        s2_vkGetInstanceProcAddr(display.instance, "vkCreateDevice");
    s2_vkGetPhysicalDeviceFormatProperties = (PFN_vkGetPhysicalDeviceFormatProperties)
        s2_vkGetInstanceProcAddr(display.instance, "vkGetPhysicalDeviceFormatProperties");
    s2_vkGetPhysicalDeviceSurfaceCapabilitiesKHR = (PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR)
        s2_vkGetInstanceProcAddr(display.instance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
    
    s2_vkGetDeviceQueue = (PFN_vkGetDeviceQueue)
        s2_vkGetInstanceProcAddr(display.instance, "vkGetDeviceQueue");
    s2_vkCreateSwapchainKHR = (PFN_vkCreateSwapchainKHR)
        s2_vkGetInstanceProcAddr(display.instance, "vkCreateSwapchainKHR");
    s2_vkGetSwapchainImagesKHR = (PFN_vkGetSwapchainImagesKHR)
        s2_vkGetInstanceProcAddr(display.instance, "vkGetSwapchainImagesKHR");
    s2_vkCreateImageView = (PFN_vkCreateImageView)
        s2_vkGetInstanceProcAddr(display.instance, "vkCreateImageView");

    s2_create_surface(display.instance, hwnd, hinstance, &display.surface);
    s2_pick_device(display.instance, display.surface, &display.physical_device, &display.queues);
    s2_create_device(display.physical_device, display.queues,
                     &display.device, &display.graphics_queue, &display.present_queue);

    // cut
    s2_vkCreateCommandPool = (PFN_vkCreateCommandPool)
        s2_vkGetInstanceProcAddr(display.instance, "vkCreateCommandPool");
    s2_vkAllocateCommandBuffers = (PFN_vkAllocateCommandBuffers)
        s2_vkGetInstanceProcAddr(display.instance, "vkAllocateCommandBuffers");
    s2_vkBeginCommandBuffer = (PFN_vkBeginCommandBuffer)
        s2_vkGetInstanceProcAddr(display.instance, "vkBeginCommandBuffer");
    s2_vkCmdPipelineBarrier = (PFN_vkCmdPipelineBarrier)
        s2_vkGetInstanceProcAddr(display.instance, "vkCmdPipelineBarrier");
    s2_vkCmdClearColorImage = (PFN_vkCmdClearColorImage)
        s2_vkGetInstanceProcAddr(display.instance, "vkCmdClearColorImage");
    s2_vkEndCommandBuffer = (PFN_vkEndCommandBuffer)
        s2_vkGetInstanceProcAddr(display.instance, "vkEndCommandBuffer");
    s2_vkCreateSemaphore = (PFN_vkCreateSemaphore)
        s2_vkGetInstanceProcAddr(display.instance, "vkCreateSemaphore");
    s2_vkCreateFence = (PFN_vkCreateFence)
        s2_vkGetInstanceProcAddr(display.instance, "vkCreateFence");
    s2_vkAcquireNextImageKHR = (PFN_vkAcquireNextImageKHR)
        s2_vkGetInstanceProcAddr(display.instance, "vkAcquireNextImageKHR");
    s2_vkQueueSubmit = (PFN_vkQueueSubmit)
        s2_vkGetInstanceProcAddr(display.instance, "vkQueueSubmit");
    s2_vkQueuePresentKHR = (PFN_vkQueuePresentKHR)
        s2_vkGetInstanceProcAddr(display.instance, "vkQueuePresentKHR");
    s2_vkWaitForFences = (PFN_vkWaitForFences)
        s2_vkGetInstanceProcAddr(display.instance, "vkWaitForFences");
    s2_vkResetFences = (PFN_vkResetFences)
        s2_vkGetInstanceProcAddr(display.instance, "vkResetFences");
    s2_vkResetCommandBuffer = (PFN_vkResetCommandBuffer)
        s2_vkGetInstanceProcAddr(display.instance, "vkResetCommandBuffer");
    
    s2_create_swapchain(&display, hwnd);
    s2_create_image_views(&display);

    EmptyWorkingSet(GetCurrentProcess());
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch (msg) {
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        (void)hdc;
        EndPaint(hwnd, &ps);
        return 0;

    case WM_SIZING: {
        RECT *r = (RECT*)lParam;
        int w = r->right - r->left;
        int h = r->bottom - r->top;
        h = (w * 9) / 16;
        r->bottom = r->top + h;
        return TRUE;
    }

    case WM_DESTROY:
        running = false;
        PostQuitMessage(0);
        return 0;

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            running = false;
            DestroyWindow(hwnd);
        }
        return 0;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{   
    WNDCLASS wc;
    HWND hwnd;
    MSG msg;
    int screenW, screenH;
    int winW, winH;
    int winX, winY;
    const char *CLASS_NAME = "s2";

    (void)hPrevInstance; (void)lpCmdLine; (void)nCmdShow;

    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nCmdShow;

    screenW = GetSystemMetrics(SM_CXSCREEN);
    screenH = GetSystemMetrics(SM_CYSCREEN);

    if (screenW * 9 >= screenH * 16) {
        winH = screenH;
        winW = (winH * 16) / 9;
    } else {
        winW = screenW;
        winH = (winW * 9) / 16;
    }

    winX = (screenW - winW) / 2;
    winY = (screenH - winH) / 2;

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = CLASS_NAME;
    
    if (!RegisterClass(&wc)) {
        MessageBox(NULL, "RegisterClass failed!", "Error", MB_ICONERROR);
        return 1;
    }

    hwnd = CreateWindowEx(WS_EX_TOPMOST, CLASS_NAME, "S2.exe", WS_POPUP,
                          winX, winY, winW, winH, NULL, NULL, hInstance, NULL);
    if (!hwnd) {
        MessageBox(NULL, "S2 failed to create a window", "Error", MB_ICONERROR);
        return 1;
    }

    s2_load_vulkan(hwnd, hInstance);
    // cut
    VkCommandPool cmd_pool;
    VkCommandBuffer cmd_buf;
    VkSemaphore sem_acquire, sem_release;
    VkFence fence;

    // command pool
    VkCommandPoolCreateInfo pool_info = {0};
    pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_info.queueFamilyIndex = display.queues.graphics_family;
    pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    s2_vkCreateCommandPool(display.device, &pool_info, NULL, &cmd_pool);

    // command buffer
    VkCommandBufferAllocateInfo alloc_info = {0};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.commandPool = cmd_pool;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandBufferCount = 1;
    s2_vkAllocateCommandBuffers(display.device, &alloc_info, &cmd_buf);

    // semaphores + fence
    VkSemaphoreCreateInfo sem_info = {0};
    sem_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    s2_vkCreateSemaphore(display.device, &sem_info, NULL, &sem_acquire);
    s2_vkCreateSemaphore(display.device, &sem_info, NULL, &sem_release);

    VkFenceCreateInfo fence_info = {0};
    fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    s2_vkCreateFence(display.device, &fence_info, NULL, &fence);

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    while (running) {

        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                running = false;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (!running)
            break;
        
        s2_vkWaitForFences(display.device, 1, &fence, VK_TRUE, UINT64_MAX);
        s2_vkResetFences(display.device, 1, &fence);

        // acquire
        uint32_t img_index;
        s2_vkAcquireNextImageKHR(display.device, display.swapchain, UINT64_MAX,
                                 sem_acquire, VK_NULL_HANDLE, &img_index);

        // record
        s2_vkResetCommandBuffer(cmd_buf, 0);
        VkCommandBufferBeginInfo begin = {0};
        begin.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        s2_vkBeginCommandBuffer(cmd_buf, &begin);

        // transition: UNDEFINED → TRANSFER_DST
        VkImageMemoryBarrier barrier = {0};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = display.images[img_index];
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.layerCount = 1;
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        s2_vkCmdPipelineBarrier(cmd_buf,
                                VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
                                0, 0, NULL, 0, NULL, 1, &barrier);

        // clear to red
        VkClearColorValue color = {{1.0f, 0.0f, 0.0f, 1.0f}};
        VkImageSubresourceRange range = {0};
        range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        range.levelCount = 1;
        range.layerCount = 1;
        s2_vkCmdClearColorImage(cmd_buf, display.images[img_index],
                                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &color, 1, &range);

        // transition: TRANSFER_DST → PRESENT_SRC
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = 0;
        s2_vkCmdPipelineBarrier(cmd_buf,
                                VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                                0, 0, NULL, 0, NULL, 1, &barrier);

        s2_vkEndCommandBuffer(cmd_buf);

        // submit
        VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        VkSubmitInfo submit = {0};
        submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit.waitSemaphoreCount = 1;
        submit.pWaitSemaphores = &sem_acquire;
        submit.pWaitDstStageMask = &wait_stage;
        submit.commandBufferCount = 1;
        submit.pCommandBuffers = &cmd_buf;
        submit.signalSemaphoreCount = 1;
        submit.pSignalSemaphores = &sem_release;
        s2_vkQueueSubmit(display.graphics_queue, 1, &submit, fence);

        // present
        VkPresentInfoKHR present = {0};
        present.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        present.waitSemaphoreCount = 1;
        present.pWaitSemaphores = &sem_release;
        present.swapchainCount = 1;
        present.pSwapchains = &display.swapchain;
        present.pImageIndices = &img_index;
        s2_vkQueuePresentKHR(display.present_queue, &present);
    }
        
    return (int)msg.wParam;
}
