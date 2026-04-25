#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include <windows.h>

#include "math/math.h"
#include "backend/vulkan.h"
#include "window.h"
#include "camera.h"

int main(void)
{
    /* vec3 pos = S2_VEC3_ZERO; */
    /* vec3 up = S2_VEC3_ZERO; */
    
    s2_window window;
    s2_window_init(&window);
    s2_window_fullscreen(&window);

    /* s2_camera camera; */
    /* s2_camera_init(&camera, &window, pos, up, 16.9f, 90.0f); */
    /* s2_camera_set_view(&camera, &window); */
    
    s2_device_context ctx = {0};
    s2_debug_utils_messenger debug_messenger;
    s2_create_instance(&ctx.instance);
    s2_setup_debug_messenger(ctx.instance, &debug_messenger);
    s2_create_suface(ctx.instance, &ctx.surface, window.id);
    s2_pick_physical_device(&ctx, ctx.instance,
                            ctx.surface, &ctx.physical_device);

    ctx.indices = s2_find_queue_families(ctx.physical_device, ctx.surface);

    s2_create_logical_device(ctx.physical_device, ctx.indices,
                             &ctx.device, &ctx.graphics_queue, &ctx.present_queue);

    ctx.swapchain = s2_create_swapchain(ctx.device, ctx.physical_device, ctx.surface, window.id,
                                                ctx.indices, &ctx.swapchain_image_format, &ctx.swapchain_extent);
    uint32_t image_count;
    vkGetSwapchainImagesKHR(ctx.device, ctx.swapchain, &image_count, NULL);
    ctx.swapchain_images = (VkImage*)malloc(sizeof(VkImage) * image_count);
    vkGetSwapchainImagesKHR(ctx.device, ctx.swapchain, &image_count, ctx.swapchain_images);

    ctx.swapchain_image_views = (VkImageView*)malloc(sizeof(VkImageView) * image_count);
    for (uint32_t i = 0; i < image_count; ++i) {
        VkImageViewCreateInfo view_info = {};
        view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        view_info.image = ctx.swapchain_images[i];
        view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        view_info.format = ctx.swapchain_image_format;
        view_info.components = (VkComponentMapping){ VK_COMPONENT_SWIZZLE_IDENTITY };
        view_info.subresourceRange = (VkImageSubresourceRange) {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1
        };

        if (vkCreateImageView(ctx.device, &view_info, NULL,
                              &ctx.swapchain_image_views[i]) != VK_SUCCESS) {
            fprintf(stderr, "Failed to create image view %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    size_t vert_size, frag_size;
    uint32_t* vert_code = s2_read_shader_file("res/shaders/vert.spv", &vert_size);
    uint32_t* frag_code = s2_read_shader_file("res/shaders/frag.spv", &frag_size);

    VkShaderModule vert_shader = s2_create_shader_module(ctx.device, vert_code, vert_size);
    VkShaderModule frag_shader = s2_create_shader_module(ctx.device, frag_code, frag_size);

    ctx.render_pass = s2_create_render_pass(ctx.device, ctx.swapchain_image_format);
    ctx.pipeline_layout = s2_create_pipeline_layout(ctx.device);
    ctx.graphics_pipeline = s2_create_graphics_pipeline(ctx.device, ctx.swapchain_extent,
                                                            vert_shader, frag_shader,
                                                            ctx.render_pass, ctx.pipeline_layout);

    ctx.framebuffers = (VkFramebuffer*)malloc(sizeof(VkFramebuffer) * image_count);
    for (uint32_t i = 0; i < image_count; ++i) {
        VkFramebufferCreateInfo fb_info = {};
        fb_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        fb_info.renderPass = ctx.render_pass;
        fb_info.attachmentCount = 1;
        fb_info.pAttachments = &ctx.swapchain_image_views[i];
        fb_info.width = ctx.swapchain_extent.width;
        fb_info.height = ctx.swapchain_extent.height;
        fb_info.layers = 1;

        if (vkCreateFramebuffer(ctx.device, &fb_info, NULL, &ctx.framebuffers[i]) != VK_SUCCESS) {
            fprintf(stderr, "Failed to create framebuffer %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    VkCommandPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_info.queueFamilyIndex = ctx.indices.graphics_family;
    pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    vkCreateCommandPool(ctx.device, &pool_info, NULL, &ctx.command_pool);

    ctx.command_buffers = (VkCommandBuffer*)malloc(sizeof(VkCommandBuffer) * image_count);
    VkCommandBufferAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.commandPool = ctx.command_pool;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandBufferCount = image_count;
    vkAllocateCommandBuffers(ctx.device, &alloc_info, ctx.command_buffers);

    VkBuffer vertex_buffer;
    VkDeviceMemory vertex_buffer_memory;
    s2_create_vertex_buffer(&ctx, &vertex_buffer, &vertex_buffer_memory);

    VkSemaphore image_available_semaphore, render_finished_semaphore;
    VkFence in_flight_fence;

    VkSemaphoreCreateInfo sem_info = {};
    sem_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    vkCreateSemaphore(ctx.device, &sem_info, NULL, &image_available_semaphore);
    vkCreateSemaphore(ctx.device, &sem_info, NULL, &render_finished_semaphore);
    
    VkFenceCreateInfo fence_info = {};
    fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    vkCreateFence(ctx.device, &fence_info, NULL, &in_flight_fence);

    // IMGUI
    VkDescriptorPoolSize pool_sizes[] = {
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, IMGUI_IMPL_VULKAN_MINIMUM_IMAGE_SAMPLER_POOL_SIZE },
    };
    
    VkDescriptorPoolCreateInfo dp_info = {};
    dp_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    dp_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    dp_info.maxSets = 1;
    
    VkDescriptorPool imgui_descriptor_pool;
    vkCreateDescriptorPool(ctx.device, &dp_info, NULL, &imgui_descriptor_pool);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    
    ImGui_ImplGlfw_InitForVulkan(window.id, true);
    
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance       = ctx.instance;
    init_info.PhysicalDevice = ctx.physical_device;
    init_info.Device         = ctx.device;
    init_info.QueueFamily    = ctx.indices.graphics_family;
    init_info.Queue          = ctx.graphics_queue;
    init_info.DescriptorPool = imgui_descriptor_pool;
    init_info.MinImageCount  = 2;
    init_info.ImageCount     = image_count;
    init_info.PipelineInfoMain.RenderPass = ctx.render_pass;
    ImGui_ImplVulkan_Init(&init_info);

    LARGE_INTEGER freq, last_time, current_time;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&last_time);

    float fps = 0.0f;
    float frame_time_ms = 0.0f;

    float angle = 0.0f;

    while (!glfwWindowShouldClose(window.id)) {
        glfwPollEvents();

        QueryPerformanceCounter(&current_time);
        frame_time_ms = (float)(current_time.QuadPart - last_time.QuadPart) * 1000.0f / freq.QuadPart;
        fps = 1000.0f / frame_time_ms;
        last_time = current_time;

        
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

        ImGui::Begin("Debug");
        
        ImGui::Text("frame rate: %.1f", fps);
        ImGui::Separator();
        ImGui::Text("frame time: %.3f ms", 1000.0f / ImGui::GetIO().Framerate);
        ImGui::End();

        ImGui::Render();
        
        vkWaitForFences(ctx.device, 1, &in_flight_fence, VK_TRUE, UINT64_MAX);
        vkResetFences(ctx.device, 1, &in_flight_fence);

        uint32_t image_index;
        VkResult result = vkAcquireNextImageKHR(ctx.device,
                                                ctx.swapchain,
                                                UINT64_MAX,
                                                image_available_semaphore,
                                                VK_NULL_HANDLE,
                                                &image_index);
        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            s2_recreate_swapchain(&ctx, window.id, image_count,
                                  &vertex_buffer, &vertex_buffer_memory);
            continue;
        }

        vkResetCommandBuffer(ctx.command_buffers[image_index], 0);

        VkCommandBufferBeginInfo begin_info = {};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        vkBeginCommandBuffer(ctx.command_buffers[image_index], &begin_info);

        VkClearValue clear_color = { .color = {{0.0f, 0.0f, 0.0f, 1.0f}} };
        VkRenderPassBeginInfo rp_info = {};
        rp_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        rp_info.renderPass = ctx.render_pass;
        rp_info.framebuffer = ctx.framebuffers[image_index];
        rp_info.renderArea.offset = (VkOffset2D){0, 0};
        rp_info.renderArea.extent = ctx.swapchain_extent;
        rp_info.clearValueCount = 1;
        rp_info.pClearValues = &clear_color;

        vkCmdBeginRenderPass(ctx.command_buffers[image_index],
                             &rp_info, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(ctx.command_buffers[image_index],
                          VK_PIPELINE_BIND_POINT_GRAPHICS, ctx.graphics_pipeline);

        VkBuffer vbs[] = { vertex_buffer };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(ctx.command_buffers[image_index], 0, 1, vbs, offsets);

        angle += 0.005f;

        float cos_a = cosf(angle);
        float sin_a = sinf(angle);

        float mvp[16] = {
            cos_a,  0.0f, sin_a, 0.0f,
            0.0f,   1.0f, 0.0f,  0.0f,
            -sin_a,  0.0f, cos_a, 0.0f,
            0.0f,   0.0f, 0.0f,  1.0f,
        };
        
        vkCmdPushConstants(ctx.command_buffers[image_index],
                   ctx.pipeline_layout,
                   VK_SHADER_STAGE_VERTEX_BIT,
                   0, sizeof(mvp), mvp);

        vkCmdDraw(ctx.command_buffers[image_index], 36, 1, 0, 0);

        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(),
                                        ctx.command_buffers[image_index]);

        vkCmdEndRenderPass(ctx.command_buffers[image_index]);
        vkEndCommandBuffer(ctx.command_buffers[image_index]);

        VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        VkSubmitInfo submit_info = {};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.waitSemaphoreCount = 1;
        submit_info.pWaitSemaphores = &image_available_semaphore; // wait for image acquisition
        submit_info.pWaitDstStageMask = wait_stages;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &ctx.command_buffers[image_index];
        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores = &render_finished_semaphore; // signal when rendering finished

        vkQueueSubmit(ctx.graphics_queue, 1, &submit_info, in_flight_fence);

        VkPresentInfoKHR present_info = {};
        present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        present_info.waitSemaphoreCount = 1;
        present_info.pWaitSemaphores = &render_finished_semaphore; // wait for rendering
        present_info.swapchainCount = 1;
        present_info.pSwapchains = &ctx.swapchain;
        present_info.pImageIndices = &image_index;

        result = vkQueuePresentKHR(ctx.present_queue, &present_info);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
            s2_recreate_swapchain(&ctx, window.id, image_count,
                                  &vertex_buffer, &vertex_buffer_memory);
            continue;
        }
    }

    vkDeviceWaitIdle(ctx.device);

    vkDestroyBuffer(ctx.device, vertex_buffer, NULL);
    vkFreeMemory(ctx.device, vertex_buffer_memory, NULL);

    vkDestroyFence(ctx.device, in_flight_fence, NULL);
    vkDestroySemaphore(ctx.device, render_finished_semaphore, NULL);
    vkDestroySemaphore(ctx.device, image_available_semaphore, NULL);

    vkFreeCommandBuffers(ctx.device, ctx.command_pool, image_count, ctx.command_buffers);
    vkDestroyCommandPool(ctx.device, ctx.command_pool, NULL);
    free(ctx.command_buffers);

    for (uint32_t i = 0; i < image_count; ++i) {
        vkDestroyFramebuffer(ctx.device, ctx.framebuffers[i], NULL);
        vkDestroyImageView(ctx.device, ctx.swapchain_image_views[i], NULL);
    }

    free(ctx.framebuffers);
    free(ctx.swapchain_image_views);
    free(ctx.swapchain_images);

    vkDestroyPipeline(ctx.device, ctx.graphics_pipeline, NULL);
    vkDestroyPipelineLayout(ctx.device, ctx.pipeline_layout, NULL);
    vkDestroyRenderPass(ctx.device, ctx.render_pass, NULL);
    
    vkDestroyShaderModule(ctx.device, vert_shader, NULL);
    vkDestroyShaderModule(ctx.device, frag_shader, NULL);
    
    free(vert_code);
    free(frag_code);

    vkDestroySwapchainKHR(ctx.device, ctx.swapchain, NULL);

    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    vkDestroyDescriptorPool(ctx.device, imgui_descriptor_pool, NULL);

    vkDestroyDevice(ctx.device, NULL);
    destroy_debug_utils_messengerEXT(ctx.instance, debug_messenger, NULL);
    vkDestroySurfaceKHR(ctx.instance, ctx.surface, NULL);
    vkDestroyInstance(ctx.instance, NULL);
    
    s2_window_clean(&window);
    glfwTerminate();

    return 0;
}
