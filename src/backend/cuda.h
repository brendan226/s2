#ifndef CUDA_H
#define CUDA_H

#include <cuda.h>
#include <stdio.h>

static void s2_cuda_init()
{
    CUdevice  cu_device;
    CUcontext cu_context;

    cuInit(0);
    cuDeviceGet(&cu_device, 0);
    cuCtxCreate(&cu_context, 0, cu_device);
}

#endif // CUDA_H
