#ifndef S2_VULKAN_H
#define S2_VULKAN_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <math.h>


typedef struct {
    float pos[3];
    float color[3];
} Vertex;

// cube
static const Vertex vertices[] = {
    {{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}},
    {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}},
    {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}},
    {{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}},
    {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}},
    {{-0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}},

    {{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},

    {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}},

    {{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 0.0f}},
    {{ 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}},
    {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}},
    {{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 0.0f}},
    {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}},
    {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 0.0f}},

    {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 1.0f}},
    {{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 1.0f}},
    {{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 1.0f}},
    {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 1.0f}},
    {{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 1.0f}},
    {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 1.0f}},

    {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 1.0f}},
    {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 1.0f}},
    {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 1.0f}},
    {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 1.0f}},
    {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 1.0f}},
    {{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 1.0f}},
};

const char *validation_layers[] = {
    "VK_LAYER_KHRONOS_validation"
};

const char *device_extensions[] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef NDEBUG
const bool enable_validation_layers = false;
#else
const bool enable_validation_layers = true;
#endif

typedef struct {
    uint32_t graphics_family;
    uint32_t present_family;
    bool     graphics_found;
    bool     present_found;
} s2_queue_family;

typedef struct {
    VkSurfaceCapabilitiesKHR capabilities;
    VkSurfaceFormatKHR       *formats;
    VkPresentModeKHR         *present_modes;
    uint32_t                 format_count;
    uint32_t                 present_mode_count;
} s2_swapchain_support;

typedef struct {
    VkImage*         swapchain_images;
    VkImageView*     swapchain_image_views;
    VkFramebuffer*   framebuffers;
    VkCommandBuffer* command_buffers;
    VkSwapchainKHR   swapchain;
    VkInstance       instance;
    VkDevice         device;
    VkSurfaceKHR     surface;
    VkPhysicalDevice physical_device;
    VkRenderPass     render_pass;
    VkCommandPool    command_pool;
    VkPipeline       graphics_pipeline;
    VkPipelineLayout pipeline_layout;
    VkExtent2D       swapchain_extent;
    VkQueue          graphics_queue;
    VkQueue          present_queue;
    VkFormat         swapchain_image_format;
    s2_queue_family  indices;
    VkPhysicalDeviceMemoryProperties mem_properties;
} s2_device_context;

typedef struct {
    VkBuffer buffer;
    VkDeviceMemory memory;
} s2_buffer;

typedef VkDebugUtilsMessengerEXT s2_debug_utils_messenger;

s2_swapchain_support query_swapchain_support(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    s2_swapchain_support details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &details.format_count, NULL);
    if (details.format_count != 0) {
        details.formats = (VkSurfaceFormatKHR*)malloc(details.format_count * sizeof(VkSurfaceFormatKHR));
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &details.format_count, details.formats);
    }

    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &details.present_mode_count, NULL);
    if (details.present_mode_count != 0) {
        details.present_modes = (VkPresentModeKHR*)malloc(details.present_mode_count * sizeof(VkPresentModeKHR));
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &details.present_mode_count, details.present_modes);
    }

    return details;
}

VkSurfaceFormatKHR s2_choose_swap_surface_format(const VkSurfaceFormatKHR* formats, uint32_t count)
{
    for (uint32_t i = 0; i < count; i++) {
        if (formats[i].format == VK_FORMAT_B8G8R8A8_SRGB &&
            formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return formats[i];
        }
    }
    return formats[0];
}

VkPresentModeKHR s2_choose_swap_present_mode(const VkPresentModeKHR* modes, uint32_t count)
{
    for (uint32_t i = 0; i < count; i++) {
        if (modes[i] == VK_PRESENT_MODE_FIFO_KHR) { // VSync On
            return modes[i];
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D s2_choose_swap_extent(const VkSurfaceCapabilitiesKHR capabilities, GLFWwindow* window)
{
    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        VkExtent2D actual_extent = {
            .width = (uint32_t)width,
            .height = (uint32_t)height
        };

        actual_extent.width = (actual_extent.width < capabilities.minImageExtent.width) ? capabilities.minImageExtent.width : actual_extent.width;
        actual_extent.width = (actual_extent.width > capabilities.maxImageExtent.width) ? capabilities.maxImageExtent.width : actual_extent.width;
        actual_extent.height = (actual_extent.height < capabilities.minImageExtent.height) ? capabilities.minImageExtent.height : actual_extent.height;
        actual_extent.height = (actual_extent.height > capabilities.maxImageExtent.height) ? capabilities.maxImageExtent.height : actual_extent.height;

        return actual_extent;
    }
}

VkResult s2_create_debug_utils_messenger(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *info,
                                      const VkAllocationCallbacks *allocator, VkDebugUtilsMessengerEXT *debug_messenger)
{
    PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func) {
        return func(instance, info, allocator, debug_messenger);
    }
    return VK_ERROR_EXTENSION_NOT_PRESENT;
}

VkSwapchainKHR s2_create_swapchain(VkDevice device, VkPhysicalDevice physical_device, VkSurfaceKHR surface,
                                   GLFWwindow* window, s2_queue_family indices, VkFormat* swapchain_image_format, VkExtent2D* swapchain_extent)
{
    s2_swapchain_support details = query_swapchain_support(physical_device, surface);

    VkSurfaceFormatKHR surface_format = s2_choose_swap_surface_format(details.formats, details.format_count);
    VkPresentModeKHR present_mode = s2_choose_swap_present_mode(details.present_modes, details.present_mode_count);
    VkExtent2D extent = s2_choose_swap_extent(details.capabilities, window);

    uint32_t image_count = details.capabilities.minImageCount + 1;
    if (details.capabilities.maxImageCount > 0 && image_count > details.capabilities.maxImageCount) {
        image_count = details.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    create_info.surface = surface;
    create_info.minImageCount = image_count;
    create_info.imageFormat = surface_format.format;
    create_info.imageColorSpace = surface_format.colorSpace;
    create_info.imageExtent = extent;
    create_info.imageArrayLayers = 1;
    create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    uint32_t queue_family_indices[] = { indices.graphics_family, indices.present_family };
    if (indices.graphics_family != indices.present_family) {
        create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        create_info.queueFamilyIndexCount = 2;
        create_info.pQueueFamilyIndices = queue_family_indices;
    } else {
        create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    create_info.preTransform = details.capabilities.currentTransform;
    create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    create_info.presentMode = present_mode;
    create_info.clipped = VK_TRUE;
    create_info.oldSwapchain = VK_NULL_HANDLE;

    VkSwapchainKHR swapchain;
    if (vkCreateSwapchainKHR(device, &create_info, NULL, &swapchain) != VK_SUCCESS) {
        fprintf(stderr, "Failed to create swapchain!\n");
        exit(EXIT_FAILURE);
    }

    *swapchain_image_format = surface_format.format;
    *swapchain_extent = extent;

    free(details.formats);
    free(details.present_modes);

    return swapchain;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL callback_data(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type,
                                                    const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data, void* p_user_data)
{
    (void)message_severity;
    (void)message_type;
    (void)p_user_data;
    fprintf(stderr, "validation layer: %s\n", p_callback_data->pMessage);
    return VK_FALSE;
}

bool s2_is_complete(s2_queue_family *indices)
{
    return indices->graphics_found && indices->present_found;
}

void destroy_debug_utils_messengerEXT(VkInstance instance,
                                      VkDebugUtilsMessengerEXT debug_messenger, const VkAllocationCallbacks* allocator)
{
    PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func) {
        func(instance, debug_messenger, allocator);
    }
}

bool s2_check_validation_layer_support()
{
    uint32_t layer_count;
    vkEnumerateInstanceLayerProperties(&layer_count, NULL);

    VkLayerProperties *available_layers = (VkLayerProperties*)malloc(layer_count * sizeof(VkLayerProperties));
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers);

    size_t count = sizeof(validation_layers) / sizeof(char*);
    for (size_t i = 0; i < count; i++) {
        bool layer_found = false;
        for (uint32_t j = 0; j < layer_count; j++) {
            if (strcmp(validation_layers[i], available_layers[j].layerName) == 0) {
                layer_found = true;
                break;
            }
        }
        if (!layer_found) {
            free(available_layers);
            return false;
        }
    }

    free(available_layers);
    return true;
}

void populate_debug_messenger(VkDebugUtilsMessengerCreateInfoEXT *info)
{
    memset(info, 0, sizeof(VkDebugUtilsMessengerCreateInfoEXT));
    info->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    info->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    info->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    info->pfnUserCallback = callback_data;        
}

const char** s2_get_required_extensions(uint32_t* extension_count)
{
    const char** glfw_extensions = glfwGetRequiredInstanceExtensions(extension_count);
    if (enable_validation_layers) {
        const char** extensions = (const char**)malloc((*extension_count + 1) * sizeof(char*));
        memcpy(extensions, glfw_extensions, *extension_count * sizeof(char*));
        extensions[*extension_count] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
        (*extension_count)++;
        return extensions;
    }
    return glfw_extensions;
}

void s2_create_instance(VkInstance *instance)
{
    if (enable_validation_layers && !s2_check_validation_layer_support()) {
        fprintf(stderr, "Validation layers requested, but not available.\n");
        exit(EXIT_FAILURE);
    }

    VkApplicationInfo app_info = {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "s2";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "s2";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;

    uint32_t extension_count = 0;
    const char **extensions = s2_get_required_extensions(&extension_count);

    create_info.enabledExtensionCount = extension_count;
    create_info.ppEnabledExtensionNames = extensions;

    VkDebugUtilsMessengerCreateInfoEXT debug_info;
    if (enable_validation_layers) {
        create_info.enabledLayerCount = 1;
        create_info.ppEnabledLayerNames = validation_layers;

        populate_debug_messenger(&debug_info);
        create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debug_info;
    } else {
        create_info.enabledLayerCount = 0;
        create_info.pNext = NULL;
    }

    if (vkCreateInstance(&create_info, NULL, instance) != VK_SUCCESS) {
        fprintf(stderr, "Failed to create Vulkan instance.\n");
        exit(EXIT_FAILURE);
    }

    free(extensions);
}

void s2_setup_debug_messenger(VkInstance instance, VkDebugUtilsMessengerEXT *debug_messenger)
{    
    if (!enable_validation_layers) return;

    VkDebugUtilsMessengerCreateInfoEXT info;
    populate_debug_messenger(&info);
    if (s2_create_debug_utils_messenger(instance, &info, NULL, debug_messenger) != VK_SUCCESS) {
        fprintf(stderr, "Failed to set up debug messenger!\n");
    }
}

s2_queue_family s2_find_queue_families(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    s2_queue_family indices = {};
    
    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, NULL);

    VkQueueFamilyProperties *queue_families = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families);

    for (uint32_t i = 0; i < queue_family_count; i++) {
        if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphics_family = i;
            indices.graphics_found = true;
        }
        VkBool32 present_support = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &present_support);
        if (present_support) {
            indices.present_family = i;
            indices.present_found = true;
        }       
        if (s2_is_complete(&indices)) break;
    }

    free(queue_families);
    return indices;
}

void s2_create_suface(VkInstance instance, VkSurfaceKHR *surface, GLFWwindow *window)
{
    if (glfwCreateWindowSurface(instance, window, NULL, surface) != VK_SUCCESS) {
        fprintf(stderr, "Failed to create window surface. %s\n");
        exit(EXIT_FAILURE);
    }
}

bool s2_is_device_suitable(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    s2_queue_family indices = s2_find_queue_families(device, surface);

    uint32_t extension_count;
    vkEnumerateDeviceExtensionProperties(device, NULL, &extension_count, NULL);

    VkExtensionProperties *available_extensions = (VkExtensionProperties*)malloc(extension_count * sizeof(VkExtensionProperties));
    vkEnumerateDeviceExtensionProperties(device, NULL, &extension_count, available_extensions);

    bool swapchain_ext_found = false;
    for (uint32_t i = 0; i < extension_count; i++) {
        if (strcmp(available_extensions[i].extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0) {
            swapchain_ext_found = true;
            break;
        }
    }

    free(available_extensions);
    return s2_is_complete(&indices) && swapchain_ext_found;
}

void s2_pick_physical_device(s2_device_context *ctx, VkInstance instance,
                             VkSurfaceKHR surface, VkPhysicalDevice *physical_device)
{
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(instance, &device_count, NULL);
    if (device_count == 0) {
        fprintf(stderr, "Failed to find GPUs with Vulkan support!\n");
        exit(EXIT_FAILURE);
    }

    VkPhysicalDevice *devices = (VkPhysicalDevice*)malloc(device_count * sizeof(VkPhysicalDevice));
    vkEnumeratePhysicalDevices(instance, &device_count, devices);

    *physical_device = VK_NULL_HANDLE;
    for (uint32_t i = 0; i < device_count; i++) {
        if (s2_is_device_suitable(devices[i], surface)) {
            *physical_device = devices[i];
            break;
        }
    }

    free(devices);

    if (*physical_device == VK_NULL_HANDLE) {
        fprintf(stderr, "Failed to find a suitable GPU!\n");
        exit(EXIT_FAILURE);
    }

    vkGetPhysicalDeviceMemoryProperties(ctx->physical_device,
                                        &ctx->mem_properties);
}

void s2_create_logical_device(VkPhysicalDevice physical_device, s2_queue_family indices,
                           VkDevice* device, VkQueue* graphics_queue, VkQueue* present_queue)
{
    VkDeviceQueueCreateInfo queue_create_infos[2];
    uint32_t unique_queue_families[2] = { indices.graphics_family, indices.present_family };
    size_t queue_count = (indices.graphics_family != indices.present_family) ? 2 : 1;

    float queue_priority = 1.0f;
    for (size_t i = 0; i < queue_count; i++) {
        queue_create_infos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_infos[i].queueFamilyIndex = unique_queue_families[i];
        queue_create_infos[i].queueCount = 1;
        queue_create_infos[i].pQueuePriorities = &queue_priority;
        queue_create_infos[i].pNext = NULL;
        queue_create_infos[i].flags = 0;
    }

    VkPhysicalDeviceFeatures device_features = {};

    VkDeviceCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_info.queueCreateInfoCount = queue_count;
    create_info.pQueueCreateInfos = queue_create_infos;
    create_info.pEnabledFeatures = &device_features;
    create_info.enabledExtensionCount = 1;
    create_info.ppEnabledExtensionNames = device_extensions;

    if (enable_validation_layers) {
        create_info.enabledLayerCount = 1;
        create_info.ppEnabledLayerNames = validation_layers;
    } else {
        create_info.enabledLayerCount = 0;
    }

    if (vkCreateDevice(physical_device, &create_info, NULL, device) != VK_SUCCESS) {
        fprintf(stderr, "Failed to create logical device.\n");
        exit(EXIT_FAILURE);
    }

    vkGetDeviceQueue(*device, indices.graphics_family, 0, graphics_queue);
    vkGetDeviceQueue(*device, indices.present_family, 0, present_queue);
}

VkShaderModule s2_create_shader_module(VkDevice device, const uint32_t* code, size_t size)
{
    VkShaderModuleCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    create_info.codeSize = size;
    create_info.pCode = code;

    VkShaderModule shader_module;
    if (vkCreateShaderModule(device, &create_info, NULL, &shader_module) != VK_SUCCESS) {
        fprintf(stderr, "Failed to create shader module!\n");
        exit(EXIT_FAILURE);
    }

    return shader_module;
}

uint32_t* s2_read_shader_file(const char* filename, size_t* file_size)
{
    FILE* file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Failed to open shader file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    if (size % 4 != 0) {
        fprintf(stderr, "Shader file size not multiple of 4 bytes!\n");
        exit(EXIT_FAILURE);
    }

    *file_size = (size_t)size;

    uint32_t* buffer = (unsigned int*)malloc(*file_size);
    fread(buffer, 1, *file_size, file);
    fclose(file);

    return buffer;
}

VkRenderPass s2_create_render_pass(VkDevice device, VkFormat swapchain_image_format)
{
    VkAttachmentDescription color_attachment = {};
    color_attachment.format = swapchain_image_format;
    color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference color_attachment_ref = {
        .attachment = 0,
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    };

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_attachment_ref;

    VkRenderPassCreateInfo render_pass_info = {};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_info.attachmentCount = 1;
    render_pass_info.pAttachments = &color_attachment;
    render_pass_info.subpassCount = 1;
    render_pass_info.pSubpasses = &subpass;

    VkRenderPass render_pass;
    if (vkCreateRenderPass(device, &render_pass_info, NULL, &render_pass) != VK_SUCCESS) {
        fprintf(stderr, "Failed to create render pass!\n");
        exit(EXIT_FAILURE);
    }

    return render_pass;
}

VkPipelineLayout s2_create_pipeline_layout(VkDevice device)
{
    VkPushConstantRange push_range = {};
    push_range.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    push_range.offset     = 0;
    push_range.size       = sizeof(float) * 16; // mat4

    VkPipelineLayoutCreateInfo layout_info = {};
    layout_info.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layout_info.pushConstantRangeCount = 1;
    layout_info.pPushConstantRanges    = &push_range;

    VkPipelineLayout layout;
    if (vkCreatePipelineLayout(device, &layout_info, NULL, &layout) != VK_SUCCESS) {
        fprintf(stderr, "Failed to create pipeline layout!\n");
        exit(EXIT_FAILURE);
    }
    return layout;
}

VkPipeline s2_create_graphics_pipeline(VkDevice device, VkExtent2D extent, VkShaderModule vert_shader,
                                    VkShaderModule frag_shader, VkRenderPass render_pass, VkPipelineLayout pipeline_layout)
{
    VkPipelineShaderStageCreateInfo vert_stage = {};
    vert_stage.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vert_stage.stage  = VK_SHADER_STAGE_VERTEX_BIT;
    vert_stage.module = vert_shader;
    vert_stage.pName  = "main";

    VkPipelineShaderStageCreateInfo frag_stage = {};
    frag_stage.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    frag_stage.stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
    frag_stage.module = frag_shader;
    frag_stage.pName  = "main";

    VkPipelineShaderStageCreateInfo shader_stages[] = { vert_stage, frag_stage };

    VkVertexInputBindingDescription binding = {};
    binding.binding   = 0;
    binding.stride    = sizeof(Vertex);
    binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    VkVertexInputAttributeDescription attrs[2] = {};
    attrs[0].binding  = 0;
    attrs[0].location = 0;
    attrs[0].format   = VK_FORMAT_R32G32B32_SFLOAT;
    attrs[0].offset   = 0;
    attrs[1].binding  = 0;
    attrs[1].location = 1;
    attrs[1].format   = VK_FORMAT_R32G32B32_SFLOAT;
    attrs[1].offset   = sizeof(float) * 3;

    VkPipelineVertexInputStateCreateInfo vertex_input_info = {};
    vertex_input_info.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_input_info.vertexBindingDescriptionCount   = 1;
    vertex_input_info.pVertexBindingDescriptions      = &binding;
    vertex_input_info.vertexAttributeDescriptionCount = 2;
    vertex_input_info.pVertexAttributeDescriptions    = attrs;

    VkPipelineInputAssemblyStateCreateInfo input_assembly = {};
    input_assembly.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_assembly.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    input_assembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport = {};
    viewport.x        = 0.0f;
    viewport.y        = 0.0f;
    viewport.width    = (float)extent.width;
    viewport.height   = (float)extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor = {
        .offset = {0, 0},
        .extent = extent
    };

    VkPipelineViewportStateCreateInfo viewport_state = {};
    viewport_state.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_state.viewportCount = 1;
    viewport_state.pViewports    = &viewport;
    viewport_state.scissorCount  = 1;
    viewport_state.pScissors     = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable        = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode             = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth               = 1.0f;
    rasterizer.cullMode                = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace               = VK_FRONT_FACE_CLOCKWISE;

    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState color_blend_attachment = {};
    color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                            VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    color_blend_attachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo color_blending = {};
    color_blending.sType           = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    color_blending.attachmentCount = 1;
    color_blending.pAttachments    = &color_blend_attachment;

    VkGraphicsPipelineCreateInfo pipeline_info = {};
    pipeline_info.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipeline_info.stageCount          = 2;
    pipeline_info.pStages             = shader_stages;
    pipeline_info.pVertexInputState   = &vertex_input_info;
    pipeline_info.pInputAssemblyState = &input_assembly;
    pipeline_info.pViewportState      = &viewport_state;
    pipeline_info.pRasterizationState = &rasterizer;
    pipeline_info.pMultisampleState   = &multisampling;
    pipeline_info.pColorBlendState    = &color_blending;
    pipeline_info.layout              = pipeline_layout;
    pipeline_info.renderPass          = render_pass;
    pipeline_info.subpass             = 0;

    VkPipeline graphics_pipeline;
    if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipeline_info, NULL, &graphics_pipeline) != VK_SUCCESS) {
        fprintf(stderr, "Failed to create graphics pipeline!\n");
        exit(EXIT_FAILURE);
    }

    return graphics_pipeline;
}

uint32_t s2_find_memory_type(
    s2_device_context *ctx,
    uint32_t type_filter,
    VkMemoryPropertyFlags properties)
{
    for (uint32_t i = 0; i < ctx->mem_properties.memoryTypeCount; i++)
        if ((type_filter & (1 << i)) &&
            (ctx->mem_properties.memoryTypes[i].propertyFlags & properties) == properties)
            return i;

    fprintf(stderr, "Failed to find suitable memory type\n");
    exit(EXIT_FAILURE);
}

void s2_cleanup_swapchain(s2_device_context *ctx, uint32_t image_count)
{
    vkFreeCommandBuffers(ctx->device, ctx->command_pool, image_count, ctx->command_buffers);

    for (uint32_t i = 0; i < image_count; i++) {
        vkDestroyFramebuffer(ctx->device, ctx->framebuffers[i], NULL);
        vkDestroyImageView(ctx->device, ctx->swapchain_image_views[i], NULL);
    }

    vkDestroySwapchainKHR(ctx->device, ctx->swapchain, NULL);

    free(ctx->command_buffers);
    free(ctx->framebuffers);
    free(ctx->swapchain_image_views);
    free(ctx->swapchain_images);

    ctx->command_buffers = NULL;
    ctx->framebuffers = NULL;
    ctx->swapchain_image_views = NULL;
    ctx->swapchain_images = NULL;
}

void s2_record_command_buffers(s2_device_context *ctx, uint32_t image_count, VkBuffer *vertex_buffer)
{
    float angle = 0.0f;
    for (uint32_t i = 0; i < image_count; i++) {

        VkCommandBufferBeginInfo begin_info = {};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        vkBeginCommandBuffer(ctx->command_buffers[i], &begin_info);

        VkClearValue clear_color = { .color = {{0.f,0.f,0.f,1.f}} };

        VkRenderPassBeginInfo rp_info = {};
        rp_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        rp_info.renderPass = ctx->render_pass;
        rp_info.framebuffer = ctx->framebuffers[i];
        rp_info.renderArea.offset = (VkOffset2D){0,0};
        rp_info.renderArea.extent = ctx->swapchain_extent;
        rp_info.clearValueCount = 1;
        rp_info.pClearValues = &clear_color;

        vkCmdBeginRenderPass(ctx->command_buffers[i], &rp_info, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(ctx->command_buffers[i],
                          VK_PIPELINE_BIND_POINT_GRAPHICS,
                          ctx->graphics_pipeline);

        VkBuffer vbs[] = { *vertex_buffer };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(ctx->command_buffers[i], 0, 1, vbs, offsets);

        angle += 0.005f;

        float cos_a = cosf(angle);
        float sin_a = sinf(angle);

        float mvp[16] = {
            cos_a,  0.0f, sin_a, 0.0f,
            0.0f,   1.0f, 0.0f,  0.0f,
            -sin_a,  0.0f, cos_a, 0.0f,
            0.0f,   0.0f, 0.0f,  1.0f,
        };
        
        vkCmdPushConstants(ctx->command_buffers[i],
                           ctx->pipeline_layout,
                           VK_SHADER_STAGE_VERTEX_BIT,
                           0, sizeof(mvp), mvp);

        vkCmdDraw(ctx->command_buffers[i], 36, 1, 0, 0);

        vkCmdEndRenderPass(ctx->command_buffers[i]);

        vkEndCommandBuffer(ctx->command_buffers[i]);
    }
}

void s2_create_buffer(
    s2_device_context *ctx,
    VkDeviceSize size,
    VkBufferUsageFlags usage,
    VkMemoryPropertyFlags properties,
    VkBuffer *buffer,
    VkDeviceMemory *memory)
{
    VkBufferCreateInfo buf_info = {};
    buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buf_info.size = size;
    buf_info.usage = usage;
    buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    vkCreateBuffer(ctx->device, &buf_info, NULL, buffer);

    VkMemoryRequirements mem_reqs;
    vkGetBufferMemoryRequirements(ctx->device, *buffer, &mem_reqs);

    VkMemoryAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.allocationSize = mem_reqs.size;
    alloc_info.memoryTypeIndex = s2_find_memory_type(
        ctx,
        mem_reqs.memoryTypeBits,
        properties
    );

    vkAllocateMemory(ctx->device, &alloc_info, NULL, memory);
    vkBindBufferMemory(ctx->device, *buffer, *memory, 0);
}

void s2_upload_to_memory(
    VkDevice device,
    VkDeviceMemory memory,
    const void *src,
    VkDeviceSize size)
{
    void *data;
    vkMapMemory(device, memory, 0, size, 0, &data);
    memcpy(data, src, (size_t)size);
    vkUnmapMemory(device, memory);
}

void s2_copy_buffer(s2_device_context *ctx,
                    VkBuffer src,
                    VkBuffer dst,
                    VkDeviceSize size)
{
    VkCommandBufferAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandPool = ctx->command_pool;
    alloc_info.commandBufferCount = 1;

    VkCommandBuffer cmd;
    vkAllocateCommandBuffers(ctx->device, &alloc_info, &cmd);

    VkCommandBufferBeginInfo begin_info = {};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(cmd, &begin_info);

    VkBufferCopy copy = {};
    copy.size = size;
    vkCmdCopyBuffer(cmd, src, dst, 1, &copy);

    vkEndCommandBuffer(cmd);

    VkSubmitInfo submit = {};
    submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit.commandBufferCount = 1;
    submit.pCommandBuffers = &cmd;

    vkQueueSubmit(ctx->graphics_queue, 1, &submit, VK_NULL_HANDLE);
    vkQueueWaitIdle(ctx->graphics_queue);

    vkFreeCommandBuffers(ctx->device, ctx->command_pool, 1, &cmd);
}

void s2_create_vertex_buffer(s2_device_context *ctx,
                             VkBuffer *vertex_buffer,
                             VkDeviceMemory *vertex_memory)
{
    VkDeviceSize size = sizeof(vertices);

    VkBuffer staging_buffer;
    VkDeviceMemory staging_memory;

    s2_create_buffer(
        ctx,
        size,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &staging_buffer,
        &staging_memory
    );

    s2_upload_to_memory(ctx->device, staging_memory, vertices, size);

    s2_create_buffer(
        ctx,
        size,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT |
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        vertex_buffer,
        vertex_memory
    );

    s2_copy_buffer(ctx, staging_buffer, *vertex_buffer, size);

    vkDestroyBuffer(ctx->device, staging_buffer, NULL);
    vkFreeMemory(ctx->device, staging_memory, NULL);
}

void s2_recreate_swapchain(s2_device_context *ctx, GLFWwindow *window, uint32_t image_count,
                           VkBuffer *vertex_buffer, VkDeviceMemory *vertex_buffer_memory)
{
    int width = 0, height = 0;
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(ctx->device);

    s2_cleanup_swapchain(ctx, image_count);

    ctx->swapchain = s2_create_swapchain(
        ctx->device,
        ctx->physical_device,
        ctx->surface,
        window,
        ctx->indices,
        &ctx->swapchain_image_format,
        &ctx->swapchain_extent
    );

    vkGetSwapchainImagesKHR(ctx->device, ctx->swapchain, &image_count, NULL);

    ctx->swapchain_images = (VkImage*)malloc(sizeof(VkImage) * image_count);
    vkGetSwapchainImagesKHR(ctx->device, ctx->swapchain, &image_count, ctx->swapchain_images);

    ctx->swapchain_image_views = (VkImageView*)malloc(sizeof(VkImageView) * image_count);

    for (uint32_t i = 0; i < image_count; i++) {
        VkImageViewCreateInfo view_info = {};
        view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        view_info.image = ctx->swapchain_images[i];
        view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        view_info.format = ctx->swapchain_image_format;

        view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        view_info.subresourceRange.baseMipLevel = 0;
        view_info.subresourceRange.levelCount = 1;
        view_info.subresourceRange.baseArrayLayer = 0;
        view_info.subresourceRange.layerCount = 1;

        vkCreateImageView(ctx->device, &view_info, NULL, &ctx->swapchain_image_views[i]);
    }

    ctx->framebuffers = (VkFramebuffer*)malloc(sizeof(VkFramebuffer) * image_count);

    for (uint32_t i = 0; i < image_count; i++) {
        VkFramebufferCreateInfo fb_info = {};
        fb_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        fb_info.renderPass = ctx->render_pass;
        fb_info.attachmentCount = 1;
        fb_info.pAttachments = &ctx->swapchain_image_views[i];
        fb_info.width = ctx->swapchain_extent.width;
        fb_info.height = ctx->swapchain_extent.height;
        fb_info.layers = 1;

        vkCreateFramebuffer(ctx->device, &fb_info, NULL, &ctx->framebuffers[i]);
    }

    ctx->command_buffers = (VkCommandBuffer*)malloc(sizeof(VkCommandBuffer) * image_count);

    VkCommandBufferAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.commandPool = ctx->command_pool;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandBufferCount = image_count;

    vkAllocateCommandBuffers(ctx->device, &alloc_info, ctx->command_buffers);

    s2_create_vertex_buffer(ctx, vertex_buffer, vertex_buffer_memory);
    
    s2_record_command_buffers(ctx, image_count, vertex_buffer);
}

#endif 
