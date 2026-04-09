#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include <math.h>

#include "vulkan.h"

uint32_t find_memory_type(VkPhysicalDevice physical_device, uint32_t type_filter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties mem_props;
    vkGetPhysicalDeviceMemoryProperties(physical_device, &mem_props);
    for (uint32_t i = 0; i < mem_props.memoryTypeCount; i++)
        if ((type_filter & (1 << i)) &&
            (mem_props.memoryTypes[i].propertyFlags & properties) == properties)
            return i;
    fprintf(stderr, "Failed to find suitable memory type\n");
    exit(EXIT_FAILURE);
}

void create_vertex_buffer(s2_renderer *renderer)
{
    VkBufferCreateInfo buf_info = {};
    buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buf_info.size  = sizeof(vertices);
    buf_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    vkCreateBuffer(renderer->device, &buf_info, NULL, &renderer->vertex_buffer);

    VkMemoryRequirements mem_reqs;
    vkGetBufferMemoryRequirements(renderer->device, renderer->vertex_buffer, &mem_reqs);

    VkMemoryAllocateInfo alloc_info = {};
    alloc_info.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.allocationSize  = mem_reqs.size;
    alloc_info.memoryTypeIndex = find_memory_type(
        renderer->physical_device,
        mem_reqs.memoryTypeBits,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );
    vkAllocateMemory(renderer->device, &alloc_info, NULL, &renderer->vertex_buffer_memory);
    vkBindBufferMemory(renderer->device, renderer->vertex_buffer, renderer->vertex_buffer_memory, 0);

    void *data;
    vkMapMemory(renderer->device, renderer->vertex_buffer_memory, 0, sizeof(vertices), 0, &data);
    memcpy(data, vertices, sizeof(vertices));
    vkUnmapMemory(renderer->device, renderer->vertex_buffer_memory);
}

int main(void)
{
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW.\n");
        return -1;
    }

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "s2v-0.0.1", NULL, NULL);
    glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    if (!window) {
        fprintf(stderr, "Failed to create window.\n");
        glfwTerminate();
        return -1;
    }

    s2_renderer renderer = {0};

    VkInstance instance;
    VkDebugUtilsMessengerEXT debug_messenger;
    create_instance(&instance);
    setup_debug_messenger(instance, &debug_messenger);
   
    if (glfwCreateWindowSurface(instance, window, NULL, &renderer.surface) != VK_SUCCESS) {
        fprintf(stderr, "Failed to create window surface.\n");
        vkDestroyInstance(instance, NULL);
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    pick_physical_device(instance, renderer.surface, &renderer.physical_device);

    renderer.indices = find_queue_families(renderer.physical_device, renderer.surface);

    VkQueue graphics_queue, present_queue;
    create_logical_device(renderer.physical_device, renderer.indices, &renderer.device, &graphics_queue, &present_queue);

    renderer.swapchain = create_swapchain(renderer.device, renderer.physical_device, renderer.surface, window,
                                                renderer.indices, &renderer.swapchain_image_format, &renderer.swapchain_extent);
    uint32_t image_count;
    vkGetSwapchainImagesKHR(renderer.device, renderer.swapchain, &image_count, NULL);
    renderer.swapchain_images = (VkImage*)malloc(sizeof(VkImage) * image_count);
    vkGetSwapchainImagesKHR(renderer.device, renderer.swapchain, &image_count, renderer.swapchain_images);

    renderer.swapchain_image_views = (VkImageView*)malloc(sizeof(VkImageView) * image_count);
    for (uint32_t i = 0; i < image_count; ++i) {
        VkImageViewCreateInfo view_info = {};
        view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        view_info.image = renderer.swapchain_images[i];
        view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        view_info.format = renderer.swapchain_image_format;
        view_info.components = (VkComponentMapping){ VK_COMPONENT_SWIZZLE_IDENTITY };
        view_info.subresourceRange = (VkImageSubresourceRange) {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1
        };

        if (vkCreateImageView(renderer.device, &view_info, NULL, &renderer.swapchain_image_views[i]) != VK_SUCCESS) {
            fprintf(stderr, "Failed to create image view %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    size_t vert_size, frag_size;
    uint32_t* vert_code = read_shader_file("res/shaders/vert.spv", &vert_size);
    uint32_t* frag_code = read_shader_file("res/shaders/frag.spv", &frag_size);

    VkShaderModule vert_shader = create_shader_module(renderer.device, vert_code, vert_size);
    VkShaderModule frag_shader = create_shader_module(renderer.device, frag_code, frag_size);

    renderer.render_pass = create_render_pass(renderer.device, renderer.swapchain_image_format);
    renderer.pipeline_layout = create_pipeline_layout(renderer.device);
    renderer.graphics_pipeline = create_graphics_pipeline(renderer.device, renderer.swapchain_extent,
                                                            vert_shader, frag_shader,
                                                            renderer.render_pass, renderer.pipeline_layout);

    renderer.framebuffers = (VkFramebuffer*)malloc(sizeof(VkFramebuffer) * image_count);
    for (uint32_t i = 0; i < image_count; ++i) {
        VkFramebufferCreateInfo fb_info = {};
        fb_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        fb_info.renderPass = renderer.render_pass;
        fb_info.attachmentCount = 1;
        fb_info.pAttachments = &renderer.swapchain_image_views[i];
        fb_info.width = renderer.swapchain_extent.width;
        fb_info.height = renderer.swapchain_extent.height;
        fb_info.layers = 1;

        if (vkCreateFramebuffer(renderer.device, &fb_info, NULL, &renderer.framebuffers[i]) != VK_SUCCESS) {
            fprintf(stderr, "Failed to create framebuffer %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    VkCommandPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_info.queueFamilyIndex = renderer.indices.graphics_family;
    pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    vkCreateCommandPool(renderer.device, &pool_info, NULL, &renderer.command_pool);

    renderer.command_buffers = (VkCommandBuffer*)malloc(sizeof(VkCommandBuffer) * image_count);
    VkCommandBufferAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.commandPool = renderer.command_pool;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandBufferCount = image_count;
    vkAllocateCommandBuffers(renderer.device, &alloc_info, renderer.command_buffers);

    create_vertex_buffer(&renderer);

    VkSemaphore image_available_semaphore, render_finished_semaphore;
    VkFence in_flight_fence;

    VkSemaphoreCreateInfo sem_info = {};
    sem_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    vkCreateSemaphore(renderer.device, &sem_info, NULL, &image_available_semaphore);
    vkCreateSemaphore(renderer.device, &sem_info, NULL, &render_finished_semaphore);
    
    VkFenceCreateInfo fence_info = {};
    fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    vkCreateFence(renderer.device, &fence_info, NULL, &in_flight_fence);

    // IMGUI
    VkDescriptorPoolSize pool_sizes[] = {
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, IMGUI_IMPL_VULKAN_MINIMUM_IMAGE_SAMPLER_POOL_SIZE },
    };
    
    VkDescriptorPoolCreateInfo dp_info = {};
    dp_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    dp_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    dp_info.maxSets = 1;
    
    VkDescriptorPool imgui_descriptor_pool;
    vkCreateDescriptorPool(renderer.device, &dp_info, NULL, &imgui_descriptor_pool);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    
    ImGui_ImplGlfw_InitForVulkan(window, true);
    
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance       = instance;
    init_info.PhysicalDevice = renderer.physical_device;
    init_info.Device         = renderer.device;
    init_info.QueueFamily    = renderer.indices.graphics_family;
    init_info.Queue          = graphics_queue;
    init_info.DescriptorPool = imgui_descriptor_pool;
    init_info.MinImageCount  = 2;
    init_info.ImageCount     = image_count;
    init_info.PipelineInfoMain.RenderPass = renderer.render_pass;
    ImGui_ImplVulkan_Init(&init_info);

    float angle = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

        ImGui::Begin("Debug");
        ImGui::Text("frame time: %.3f ms", 1000.0f / ImGui::GetIO().Framerate);
        ImGui::End();

        ImGui::Render();
        
        vkWaitForFences(renderer.device, 1, &in_flight_fence, VK_TRUE, UINT64_MAX);
        vkResetFences(renderer.device, 1, &in_flight_fence);

        uint32_t image_index;
        VkResult result = vkAcquireNextImageKHR(renderer.device,
                                                renderer.swapchain,
                                                UINT64_MAX,
                                                image_available_semaphore,
                                                VK_NULL_HANDLE,
                                                &image_index);
        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            /* recreate_swapchain(&renderer, window, image_count); */
            continue;
        }

        vkResetCommandBuffer(renderer.command_buffers[image_index], 0);

        VkCommandBufferBeginInfo begin_info = {};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        vkBeginCommandBuffer(renderer.command_buffers[image_index], &begin_info);

        VkClearValue clear_color = { .color = {{0.0f, 0.0f, 0.0f, 1.0f}} };
        VkRenderPassBeginInfo rp_info = {};
        rp_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        rp_info.renderPass = renderer.render_pass;
        rp_info.framebuffer = renderer.framebuffers[image_index];
        rp_info.renderArea.offset = (VkOffset2D){0, 0};
        rp_info.renderArea.extent = renderer.swapchain_extent;
        rp_info.clearValueCount = 1;
        rp_info.pClearValues = &clear_color;

        vkCmdBeginRenderPass(renderer.command_buffers[image_index], &rp_info, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(renderer.command_buffers[image_index], VK_PIPELINE_BIND_POINT_GRAPHICS, renderer.graphics_pipeline);

        VkBuffer vbs[] = { renderer.vertex_buffer };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(renderer.command_buffers[image_index], 0, 1, vbs, offsets);

        angle += 0.005f;

        float cos_a = cosf(angle);
        float sin_a = sinf(angle);

        float mvp[16] = {
            cos_a,  0.0f, sin_a, 0.0f,
            0.0f,   1.0f, 0.0f,  0.0f,
            -sin_a,  0.0f, cos_a, 0.0f,
            0.0f,   0.0f, 0.0f,  1.0f,
        };
        
        vkCmdPushConstants(renderer.command_buffers[image_index],
                   renderer.pipeline_layout,
                   VK_SHADER_STAGE_VERTEX_BIT,
                   0, sizeof(mvp), mvp);

        vkCmdDraw(renderer.command_buffers[image_index], 36, 1, 0, 0);

        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), renderer.command_buffers[image_index]);

        vkCmdEndRenderPass(renderer.command_buffers[image_index]);
        vkEndCommandBuffer(renderer.command_buffers[image_index]);

        VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        VkSubmitInfo submit_info = {};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.waitSemaphoreCount = 1;
        submit_info.pWaitSemaphores = &image_available_semaphore; // wait for image acquisition
        submit_info.pWaitDstStageMask = wait_stages;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &renderer.command_buffers[image_index];
        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores = &render_finished_semaphore; // signal when rendering finished

        vkQueueSubmit(graphics_queue, 1, &submit_info, in_flight_fence);

        VkPresentInfoKHR present_info = {};
        present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        present_info.waitSemaphoreCount = 1;
        present_info.pWaitSemaphores = &render_finished_semaphore; // wait for rendering
        present_info.swapchainCount = 1;
        present_info.pSwapchains = &renderer.swapchain;
        present_info.pImageIndices = &image_index;

        result = vkQueuePresentKHR(present_queue, &present_info);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
            /* recreate_swapchain(&renderer, window, image_count); */
            continue;
        }
    }

    vkDeviceWaitIdle(renderer.device);

    vkDestroyBuffer(renderer.device, renderer.vertex_buffer, NULL);
    vkFreeMemory(renderer.device, renderer.vertex_buffer_memory, NULL);

    vkDestroyFence(renderer.device, in_flight_fence, NULL);
    vkDestroySemaphore(renderer.device, render_finished_semaphore, NULL);
    vkDestroySemaphore(renderer.device, image_available_semaphore, NULL);

    vkFreeCommandBuffers(renderer.device, renderer.command_pool, image_count, renderer.command_buffers);
    vkDestroyCommandPool(renderer.device, renderer.command_pool, NULL);
    free(renderer.command_buffers);

    for (uint32_t i = 0; i < image_count; ++i) {
        vkDestroyFramebuffer(renderer.device, renderer.framebuffers[i], NULL);
        vkDestroyImageView(renderer.device, renderer.swapchain_image_views[i], NULL);
    }

    free(renderer.framebuffers);
    free(renderer.swapchain_image_views);
    free(renderer.swapchain_images);

    vkDestroyPipeline(renderer.device, renderer.graphics_pipeline, NULL);
    vkDestroyPipelineLayout(renderer.device, renderer.pipeline_layout, NULL);
    vkDestroyRenderPass(renderer.device, renderer.render_pass, NULL);
    
    vkDestroyShaderModule(renderer.device, vert_shader, NULL);
    vkDestroyShaderModule(renderer.device, frag_shader, NULL);
    
    free(vert_code);
    free(frag_code);

    vkDestroySwapchainKHR(renderer.device, renderer.swapchain, NULL);

    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    vkDestroyDescriptorPool(renderer.device, imgui_descriptor_pool, NULL);

    vkDestroyDevice(renderer.device, NULL);
    destroy_debug_utils_messengerEXT(instance, debug_messenger, NULL);
    vkDestroySurfaceKHR(instance, renderer.surface, NULL);
    vkDestroyInstance(instance, NULL);
    
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
