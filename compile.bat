@echo
cd lib/vulkan/Bin/

:: Compile vertex shader
slangc.exe ../../../res/shaders/triangle.vert ^
    -target spirv ^
    -profile spirv_1_3 ^
    -emit-spirv-directly ^
    -o ../../../res/shaders/vert.spv

:: Compile fragment shader
slangc.exe ../../../res/shaders/triangle.frag ^
    -target spirv ^
    -profile spirv_1_3 ^
    -emit-spirv-directly ^
    -o ../../../res/shaders/frag.spv
