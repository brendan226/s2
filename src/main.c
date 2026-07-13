#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <windows.h>
#include <cuda.h>
#include <psapi.h>

#include "display.h"

#define S2_REJECT_MS_BLOAT(x) EmptyWorkingSet(x)
#define S2_PROGRAM(x) GetCurrentProcess(x)

static bool running = true;

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
    WNDCLASS wc  = {0};
    HWND hwnd;
    MSG msg;
    int screenW, screenH;
    int winW, winH;
    int winX, winY;
    const char *CLASS_NAME = "s2";

    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nCmdShow;
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
    wc.hCursor = SetCursor(LoadCursor(NULL, IDC_ARROW));
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(1));
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

    // ALLOCATE VRAM WITH CUDA
    cuInit(0);

    CUdevice device;
    CUctxCreateParams params = {0};
    cuDeviceGet(&device, 0);

    CUcontext context;
    cuCtxCreate(&context, &params, 0, device);

    CUdeviceptr ptr;
    size_t size = 4096;

    CUresult result = cuMemAlloc(&ptr, size);

    if (result == CUDA_SUCCESS) {
        printf("Allocated %zu bytes at GPU address 0x%llx\n",
               size,
               (unsigned long long)ptr);

        cuMemFree(ptr);
    }

    cuCtxDestroy(context);

    S2_REJECT_MS_BLOAT(S2_PROGRAM());

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
    }
        
    return (int)msg.wParam;
}
