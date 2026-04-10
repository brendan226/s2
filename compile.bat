@echo

@cd lib/vulkan/Bin/

@echo S2 loading shaders:
slangc.exe ../../../res/shaders/cube.vert ^
    -target spirv ^
    -profile spirv_1_3 ^
    -emit-spirv-directly ^
    -o ../../../res/shaders/vert.spv

slangc.exe ../../../res/shaders/cube.frag ^
    -target spirv ^
    -profile spirv_1_3 ^
    -emit-spirv-directly ^
    -o ../../../res/shaders/frag.spv

@echo.
