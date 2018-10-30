#include "Renderer.h"
#include "ShaderLib.h"
#include "Texture.h"
#include "TextureAtlas.h"
#include "Vertex.h"
#include "Logger.h"
#include "telemetry.h"
#include "BoundBuffer.h"
#include "PhysicalDevice.h"

#include <memory>
#include <utility>
#include <iostream>
#include <fstream>
#include <set>

#pragma clang diagnostic push

#pragma clang diagnostic ignored "-Wreturn-stack-address"
#pragma ide diagnostic ignored "cert-err58-cpp"

#define UNUSED(x) (void(x))


namespace {
    auto logger = GetLogger("renderer");

    const std::vector<const char *> validationLayers = {
            "VK_LAYER_LUNARG_standard_validation"
    };

    const std::vector<const char *> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    std::vector<const char *> getRequiredExtensions(bool enableValidationLayers) {
        uint32_t glfwExtensionCount = 0;
        const char **glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (enableValidationLayers) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
    }

    bool checkValidationLayerSupport() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char *layerName : validationLayers) {
            bool layerFound = false;

            for (const auto &layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    }

    bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

        for (const auto &extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                          const VkAllocationCallbacks *pAllocator,
                                          VkDebugUtilsMessengerEXT *pCallback) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance,
                                                                               "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pCallback);
        } else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback,
                                       const VkAllocationCallbacks *pAllocator) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance,
                                                                                "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(instance, callback, pAllocator);
        }
    }

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) {
        if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
            return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
        }

        for (const auto &availableFormat : availableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM &&
                availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes) {
        VkPresentModeKHR bestMode = VK_PRESENT_MODE_IMMEDIATE_KHR;

        for (const auto &availablePresentMode : availablePresentModes) {
            logger->debug("availablePresentMode {}", availablePresentMode);
            if (availablePresentMode == VK_PRESENT_MODE_FIFO_RELAXED_KHR) {
                logger->debug("Found VK_PRESENT_MODE_FIFO_RELAXED_KHR");
                return availablePresentMode;
            } else if (availablePresentMode == VK_PRESENT_MODE_FIFO_KHR) {
                bestMode = availablePresentMode;
                logger->debug("Found VK_PRESENT_MODE_FIFO_KHR");
            }
        }

        return bestMode;
    }

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, GLFWwindow *window) {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return capabilities.currentExtent;
        } else {
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);

            VkExtent2D actualExtent = {
                    static_cast<uint32_t>(width),
                    static_cast<uint32_t>(height)
            };

            actualExtent.width = std::max(capabilities.minImageExtent.width,
                                          std::min(capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height = std::max(capabilities.minImageExtent.height,
                                           std::min(capabilities.maxImageExtent.height, actualExtent.height));

            return actualExtent;
        }
    }

}

Renderer::Renderer() :
    m_indexWriteStart(nullptr),
    m_currentIndexWrite(nullptr),
    m_indexWriteEnd(nullptr),
    m_vertexWriteStart(nullptr),
    m_currentVertexWrite(nullptr),
    m_vertexWriteEnd(nullptr),
    m_currentCommandBuffer(nullptr),
    m_currentBlendMode(BM_NONE)
{
    logger->debug("Constructed renderer");
}

Renderer::~Renderer() {
    if (m_isInitialized) {
        cleanup();
    }
    logger->debug("Destroyed renderer");
}

bool Renderer::IsInitialized() { return m_isInitialized; }

void Renderer::Initialize(GLFWwindow *window, Renderer::ValidationState validation) {
    if (m_isInitialized) {
        throw std::runtime_error("renderer is already initialized");
    }

    m_window = window;
    m_enableValidationLayers = validation == ENABLE_VALIDATION;

    createInstance();

    if (m_enableValidationLayers) {
        setupDebugCallback();
    }

    setSurfaceFromWindow(window);
    pickPhysicalDevice();
    createLogicalDevice();
    createSwapChain();
    createCommandPool();
    createSyncObjects();
    createIndexAndVertexBuffers();
    m_buffersToDestroyLater.resize(getMaxFramesInFlight());

    m_pipelineSprites.Initialize(m_device, m_physicalDevice, m_swapChainExtent, m_swapChain,
                                 m_swapChainImageFormat);

    uint8_t whitePixel[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    m_defaultTexture = std::make_shared<Texture>(this, 1, 1, &whitePixel[0]);


    m_isInitialized = true;
}

void Renderer::createInstance() {
    if (m_enableValidationLayers && !checkValidationLayerSupport()) {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan Sprites";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Vulkan Sprites";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    auto extensions = getRequiredExtensions(m_enableValidationLayers);
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    if (m_enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
}

void Renderer::setupDebugCallback() {
    m_debugMessenger = std::make_unique<DebugMessenger>();

    VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = DebugMessenger::Log;
    createInfo.pUserData = reinterpret_cast<void*>(m_debugMessenger.get());

    if (CreateDebugUtilsMessengerEXT(m_instance, &createInfo, nullptr, &m_callback) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug callback!");
    }
}

void Renderer::setSurfaceFromWindow(GLFWwindow *window) {
    if (glfwCreateWindowSurface(m_instance, window, nullptr, &m_surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
}

void Renderer::pickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

    for (const auto &device : devices) {
        if (isDeviceSuitable(device)) {
            m_physicalDevice = device;
            break;
        }
    }

    if (m_physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

Renderer::QueueFamilyIndices Renderer::findQueueFamilies(VkPhysicalDevice device) {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    uint32_t i = 0;
    for (const auto &queueFamily : queueFamilies) {
        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = 0;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface, &presentSupport);

        if (queueFamily.queueCount > 0 && presentSupport) {
            indices.presentFamily = i;
        }

        if (indices.isComplete()) {
            break;
        }

        i++;
    }

    return indices;
}

bool Renderer::isDeviceSuitable(VkPhysicalDevice device) {
    QueueFamilyIndices indices = findQueueFamilies(device);

    bool extensionsSupported = checkDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

    return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

Renderer::SwapChainSupportDetails Renderer::querySwapChainSupport(VkPhysicalDevice device) {
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount,
                                                  details.presentModes.data());
    }

    return details;
}

void Renderer::createLogicalDevice() {
    QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily, indices.presentFamily};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures = {};
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (m_enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(m_device, indices.graphicsFamily, 0, &m_graphicsQueue);
    vkGetDeviceQueue(m_device, indices.presentFamily, 0, &m_presentQueue);
}

void Renderer::createSwapChain() {
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(m_physicalDevice);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, m_window);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 &&
        imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = m_surface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice);
    uint32_t queueFamilyIndices[] = {(uint32_t) indices.graphicsFamily, (uint32_t) indices.presentFamily};

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    if (vkCreateSwapchainKHR(m_device, &createInfo, nullptr, &m_swapChain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }

    m_swapChainImageFormat = surfaceFormat.format;
    m_swapChainExtent = extent;
}

void Renderer::createImageViews() {
    m_swapChainImageViews.resize(getMaxFramesInFlight());

    for (size_t i = 0; i < getMaxFramesInFlight(); i++) {
        m_swapChainImageViews[i] = createImageView(m_swapChainImages[i], m_swapChainImageFormat);
    }
}

uint32_t Renderer::getMaxFramesInFlight() {
    return 3; // TODO static_cast<uint32_t>(m_swapChainImages.size());
}

VkImageView Renderer::createImageView(VkImage image, VkFormat format) {
    VkImageViewCreateInfo viewInfo = {};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;
    if (vkCreateImageView(m_device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture image view!");
    }

    return imageView;
}



void Renderer::createCommandPool() {
    QueueFamilyIndices queueFamilyIndices = findQueueFamilies(m_physicalDevice);

    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;

    if (vkCreateCommandPool(m_device, &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics command pool!");
    }

    auto framesInFlight = getMaxFramesInFlight();
    m_perFrameCommandPool.resize(framesInFlight);
    m_perFrameCommandBuffer.resize(framesInFlight);
    for (int i = 0; i < framesInFlight; ++i) {
        if (vkCreateCommandPool(m_device, &poolInfo, nullptr, &m_perFrameCommandPool[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create graphics command pool!");
        }
    }
}

std::shared_ptr<Texture> Renderer::CreateTexture(const std::string &filename) {
    return std::make_shared<Texture>(this, filename);
}

std::shared_ptr<TextureAtlas> Renderer::CreateTextureAtlas(uint32_t width, uint32_t height) {
    auto ta = std::make_shared<TextureAtlas>();
    ta->Initialize(this, width, height);
    return ta;
}

void Renderer::CopyToBufferMemory(VkDeviceMemory bufferMemory, uint8_t *data, VkDeviceSize size) {
    void *mapped = nullptr;
    vkMapMemory(m_device, bufferMemory, 0, size, 0, &mapped);
    memcpy(mapped, data, size);
    vkUnmapMemory(m_device, bufferMemory);
}

BoundImage Renderer::CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
                      VkMemoryPropertyFlags properties) {
    VkImageCreateInfo imageInfo = {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkImage image;
    VkDeviceMemory imageMemory;
    if (vkCreateImage(m_device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
        throw std::runtime_error("failed to create image");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(m_device, image, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(m_physicalDevice, memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(m_device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate image memory");
    }

    vkBindImageMemory(m_device, image, imageMemory, 0);

    return BoundImage{image, imageMemory};
}

void Renderer::TransitionImageLayout(VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout) {
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkImageMemoryBarrier barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    VkPipelineStageFlags destinationStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

    switch (oldLayout) {
        case VK_IMAGE_LAYOUT_UNDEFINED:
            barrier.srcAccessMask = 0;
            break;
        case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            break;
        case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
            barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
            sourceStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            break;

        default:
            throw std::runtime_error("unsupported image transition");
    }

    switch (newLayout) {
        case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            break;
        case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            break;

        default:
            throw std::runtime_error("unsupported image transition");
    }
    vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
    endSingleTimeCommands(commandBuffer);
}

VkCommandBuffer Renderer::beginSingleTimeCommands() {
    if(m_currentCommandBuffer) {
        return m_currentCommandBuffer;
    }

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = m_commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(m_device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void Renderer::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
    if(commandBuffer == m_currentCommandBuffer) {
        return;
    }

    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(m_graphicsQueue);

    vkFreeCommandBuffers(m_device, m_commandPool, 1, &commandBuffer);
}

void Renderer::CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, int offsetX,
                                 int offsetY) {
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkBufferImageCopy region = {};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = {offsetX, offsetY, 0};
    region.imageExtent = {width, height, 1};

    vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
    endSingleTimeCommands(commandBuffer);
}

void Renderer::DestroyBuffer(BoundBuffer buffer) {
    vkDestroyBuffer(m_device, buffer.buffer, nullptr);
    vkFreeMemory(m_device, buffer.bufferMemory, nullptr);
}

VkImageView Renderer::CreateImageView(VkImage image, VkFormat format) {
    VkImageViewCreateInfo viewInfo = {};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;
    if (vkCreateImageView(m_device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture image view");
    }

    return imageView;
}

// TODO: Samplers can be shared. Decide on parameters and reuse samplers
// when the same parameters are passed.
VkSampler Renderer::CreateSampler() {
    VkSamplerCreateInfo samplerInfo = {};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = 16;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

    VkSampler sampler;
    if (vkCreateSampler(m_device, &samplerInfo, nullptr, &sampler) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture sampler");
    }

    return sampler;
}




void Renderer::createSyncObjects() {
    m_imageAvailableSemaphores.resize(getMaxFramesInFlight());
    m_renderFinishedSemaphores.resize(getMaxFramesInFlight());
    m_inFlightFences.resize(getMaxFramesInFlight());

    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < getMaxFramesInFlight(); ++i) {
        if (vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_imageAvailableSemaphores[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create semaphore for frame");
        }
        if (vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_renderFinishedSemaphores[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create semaphore for frame");
        }
        if (vkCreateFence(m_device, &fenceInfo, nullptr, &m_inFlightFences[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create fence for frame");
        }
    }
}

void Renderer::cleanup() {
    m_defaultTexture.reset();

    cleanupPendingDestroyBuffers();
    cleanupSwapChain();
    cleanupSyncObjects();
    cleanupIndexAndVertexBuffers();
    cleanupCommandPools();

    m_pipelineSprites.Cleanup();

    vkDestroyDevice(m_device, nullptr);

    if (m_enableValidationLayers) {
        DestroyDebugUtilsMessengerEXT(m_instance, m_callback, nullptr);
    }

    vkDestroyInstance(m_instance, nullptr);
}

void Renderer::cleanupPendingDestroyBuffers() {
    for(auto& buffers: m_buffersToDestroyLater) {
        for(auto buffer: buffers) {
            DestroyBuffer(buffer);
        }
    }
}

void Renderer::cleanupSwapChain() {
    for (auto framebuffer: m_swapChainFramebuffers) {
        vkDestroyFramebuffer(m_device, framebuffer, nullptr);
    }

    for (size_t i = 0; i < getMaxFramesInFlight(); ++i) {
        auto &perFrameCmds = m_perFrameCommandBuffer[i];
        if (!perFrameCmds.empty()) {
            vkFreeCommandBuffers(
                    m_device,
                    m_perFrameCommandPool[i],
                    static_cast<uint32_t>(perFrameCmds.size()),
                    perFrameCmds.data());
            perFrameCmds.clear();
        }
    }

    m_currentCommandBuffer = nullptr;

    vkDestroySwapchainKHR(m_device, m_swapChain, nullptr);
}

void Renderer::cleanupSyncObjects() {
    for (size_t i = 0; i < getMaxFramesInFlight(); ++i) {
        vkDestroySemaphore(m_device, m_imageAvailableSemaphores[i], nullptr);
        vkDestroySemaphore(m_device, m_renderFinishedSemaphores[i], nullptr);
        vkDestroyFence(m_device, m_inFlightFences[i], nullptr);
    }
}

void Renderer::cleanupCommandPools() {
    vkDestroyCommandPool(m_device, m_commandPool, nullptr);

    for (size_t i = 0; i < getMaxFramesInFlight(); ++i) {
        vkDestroyCommandPool(m_device, m_perFrameCommandPool[i], nullptr);
    }
}

void Renderer::DestroyImageView(VkImageView imageView) {
    vkDestroyImageView(m_device, imageView, nullptr);
}

void Renderer::DestroySampler(VkSampler sampler) {
    vkDestroySampler(m_device, sampler, nullptr);
}

void Renderer::DestroyImage(BoundImage image) {
    vkDestroyImage(m_device, image.image, nullptr);
    vkFreeMemory(m_device, image.imageMemory, nullptr);
}

void Renderer::WaitUntilDeviceIdle() {
    vkDeviceWaitIdle(m_device);
}

bool Renderer::StartFrame() {
    tmFunction(0, 0);

    {
        tmZone(0, 0, "wait for inflight fences");
        vkWaitForFences(
                m_device,
                1,
                &m_inFlightFences[m_currentFrame],
                VK_TRUE,
                std::numeric_limits<uint64_t>::max());
    }

    {
        tmZone(0, 0, "vkAcquireNextImageKHR");
        VkResult result = vkAcquireNextImageKHR(
                m_device,
                m_swapChain,
                std::numeric_limits<uint64_t>::max(),
                m_imageAvailableSemaphores[m_currentFrame],
                VK_NULL_HANDLE,
                &m_nextFrame);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return false;
        } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image");
        }
    }

    auto &perFrameCmds = m_perFrameCommandBuffer[m_currentFrame];
    if (!perFrameCmds.empty()) {
        tmZone(0, 0, "vkFreeCommandBuffers");
        vkFreeCommandBuffers(
                m_device,
                m_perFrameCommandPool[m_currentFrame],
                static_cast<uint32_t>(perFrameCmds.size()),
                &perFrameCmds[0]);
        perFrameCmds.clear();
    }

    for(auto buffer: m_buffersToDestroyLater[m_currentFrame]) {
        DestroyBuffer(buffer);
    }
    m_buffersToDestroyLater[m_currentFrame].clear();

    m_currentDescriptorSet = m_defaultTexture->GetDescriptorSet();
    m_currentTextureWindow = m_defaultTexture->GetTextureWindow();
    m_currentColor = {1.0f, 1.0f, 1.0f, 1.0f};

    m_currentBufferIndex = 0;

    startMainCommandBuffer();

    return true;
}

void Renderer::EndFrame() {
    tmFunction(0, 0);

    queueCurrentBatch();
    drawBatches();

    if (vkEndCommandBuffer(m_currentCommandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer");
    }

    VkSemaphore waitSemaphores[] = {m_imageAvailableSemaphores[m_currentFrame]};
    VkSemaphore signalSemaphores[] = {m_renderFinishedSemaphores[m_currentFrame]};

    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_currentCommandBuffer;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkResetFences(m_device, 1, &m_inFlightFences[m_currentFrame]);

    {
        tmZone(0, 0, "vkQueueSubmit");
        if (vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, m_inFlightFences[m_currentFrame]) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit command buffer");
        }
    }

    m_currentCommandBuffer = nullptr;

    VkSwapchainKHR swapChains[] = {m_swapChain};
    uint32_t imageIndices[] = {m_nextFrame};

    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = imageIndices;

    {
        tmZone(0, 0, "vkQueuePresentKHR");
        auto result = vkQueuePresentKHR(m_presentQueue, &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
            recreateSwapChain();
        } else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image");
        }
    }

    m_currentFrame++;
    m_currentFrame %= getMaxFramesInFlight();
}

void Renderer::queueCurrentBatch() {
    queueDrawCommand();
    copyStagingBuffersToDevice(m_currentCommandBuffer);
    unmapStagingBuffers();

    m_currentBufferIndex++;
}

void Renderer::drawBatches() {
    m_pipelineSprites.BeginRenderPass(m_currentCommandBuffer, m_nextFrame);

    if(!m_drawCommands.empty()) {
        int curBuffer = -1;

        for(auto& cmd: m_drawCommands) {
            if(cmd.bufferIndex != curBuffer) {
                curBuffer = cmd.bufferIndex;
                logger->debug("Binding {}, {}", m_currentFrame, curBuffer);

                VkBuffer vertexBuffers[] = {m_vertexBuffers[m_currentFrame][curBuffer].buffer};
                VkDeviceSize offsets[] = {0};

                vkCmdBindVertexBuffers(
                        m_currentCommandBuffer,
                        0,
                        1,
                        vertexBuffers,
                        offsets);
                vkCmdBindIndexBuffer(
                        m_currentCommandBuffer,
                        m_indexBuffers[m_currentFrame][curBuffer].buffer,
                        0,
                        VK_INDEX_TYPE_UINT16);
            }

            std::array<VkDescriptorSet, 2> descriptorSets = {
                    m_pipelineSprites.GetPerFrameDescriptorSet(m_currentFrame),
                    cmd.descriptorSet
            };
            vkCmdBindDescriptorSets(m_currentCommandBuffer,
                                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                                    m_pipelineSprites.GetPipelineLayout(),
                                    0,
                                    descriptorSets.size(),
                                    descriptorSets.data(),
                                    0, nullptr);

            vkCmdDrawIndexed(m_currentCommandBuffer, cmd.numIndices, 1, cmd.baseIndex, 0, 0);

        }
    }
    m_pipelineSprites.EndRenderPass(m_currentCommandBuffer);

    m_numDrawCommands = m_drawCommands.size();
    m_drawCommands.clear();
}

void Renderer::copyStagingBuffersToDevice(VkCommandBuffer commandBuffer) {
    tmFunction(0, 0);

    VkDeviceSize indexSize = (m_currentIndexWrite - m_indexWriteStart) * sizeof(uint16_t);
    if(indexSize > 0) {
        auto ib = getIndexBuffer();
        auto isb = getIndexStagingBuffer();

        VkBufferCopy copyRegion = {};
        copyRegion.size = indexSize;
        vkCmdCopyBuffer(commandBuffer, isb.buffer, ib.buffer, 1, &copyRegion);
    }

    VkDeviceSize vertexSize = (m_currentVertexWrite - m_vertexWriteStart) * sizeof(Vertex);
    if(vertexSize > 0) {
        auto vb = getVertexBuffer();
        auto vsb = getVertexStagingBuffer();

        VkBufferCopy copyRegion = {};
        copyRegion.size = vertexSize;
        vkCmdCopyBuffer(commandBuffer, vsb.buffer, vb.buffer, 1, &copyRegion);
    }
}

BoundBuffer& Renderer::getVertexBuffer() {
    auto& vertexBuffers = m_vertexBuffers[m_currentFrame];
    if(m_currentBufferIndex == vertexBuffers.size()) {
        logger->debug("Creating vertex buffer ({}, {})", m_currentFrame, m_currentBufferIndex);
        auto buffer = CreateBuffer(
                getVertexBufferSize(),
                VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        vertexBuffers.push_back(buffer);
    }
    if(m_currentBufferIndex > vertexBuffers.size()) {
        throw std::runtime_error("current buffer index value out of range");
    }
    auto& vb = vertexBuffers[m_currentBufferIndex];
    return vb;
}

BoundBuffer& Renderer::getIndexBuffer() {
    auto& indexBuffers = m_indexBuffers[m_currentFrame];
    if(m_currentBufferIndex == indexBuffers.size()) {
        logger->debug("Creating index buffer ({}, {})", m_currentFrame, m_currentBufferIndex);
        auto buffer = CreateBuffer(
                getIndexBufferSize(),
                VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        indexBuffers.push_back(buffer);
    }
    if(m_currentBufferIndex > indexBuffers.size()) {
        throw std::runtime_error("current buffer index value out of range");
    }
    auto& ib = indexBuffers[m_currentBufferIndex];
    return ib;
}

BoundBuffer& Renderer::getVertexStagingBuffer() {
    auto& vertexStagingBuffers = m_vertexStagingBuffers[m_currentFrame];
    if(m_currentBufferIndex == vertexStagingBuffers.size()) {
        logger->debug("Creating vertex staging buffer ({}, {})", m_currentFrame, m_currentBufferIndex);
        auto buffer = CreateBuffer(
                getVertexBufferSize(),
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        vertexStagingBuffers.push_back(buffer);
    }
    if(m_currentBufferIndex > vertexStagingBuffers.size()) {
        throw std::runtime_error("current buffer index value out of range");
    }
    auto& vsb = vertexStagingBuffers[m_currentBufferIndex];
    return vsb;
}

BoundBuffer& Renderer::getIndexStagingBuffer() {
    auto& indexStagingBuffers = m_indexStagingBuffers[m_currentFrame];
    if(m_currentBufferIndex == indexStagingBuffers.size()) {
        logger->debug("Creating index staging buffer ({}, {})", m_currentFrame, m_currentBufferIndex);
        auto buffer = CreateBuffer(
                getIndexBufferSize(),
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        indexStagingBuffers.push_back(buffer);
    }
    if(m_currentBufferIndex > indexStagingBuffers.size()) {
        throw std::runtime_error("current buffer index value out of range");
    }
    auto& isb = indexStagingBuffers[m_currentBufferIndex];
    return isb;
}

void Renderer::mapStagingBufferMemory() {
    void* data;

    auto isb = getIndexStagingBuffer();
    vkMapMemory(m_device, isb.bufferMemory, 0, getIndexBufferSize(), 0, &data);
    m_indexWriteStart = reinterpret_cast<uint16_t*>(data);
    m_currentIndexWrite = m_indexWriteStart;
    m_indexWriteEnd =  m_indexWriteStart + getMaxNumIndices();

    auto vsb = getVertexStagingBuffer();
    vkMapMemory(m_device, vsb.bufferMemory, 0, getVertexBufferSize(), 0, &data);
    m_vertexWriteStart = reinterpret_cast<Vertex*>(data);
    m_currentVertexWrite = m_vertexWriteStart;
    m_vertexWriteEnd = m_vertexWriteStart + getMaxNumVertices();

    m_numIndices = 0;
    m_indexOffset = 0;
    m_numVertices = 0;
    m_vertexOffset = 0;
}

void Renderer::unmapStagingBuffers() {
    tmFunction(0, 0);

    if(!m_indexWriteStart) {
        return;
    }
    auto isb = getIndexStagingBuffer();
    vkUnmapMemory(m_device, isb.bufferMemory);
    m_indexWriteStart = nullptr;
    m_currentIndexWrite = nullptr;
    m_indexWriteEnd = nullptr;

    auto vsb = getVertexStagingBuffer();
    vkUnmapMemory(m_device, vsb.bufferMemory);
    m_vertexWriteStart = nullptr;
    m_currentVertexWrite = nullptr;
    m_vertexWriteEnd = nullptr;
}

void Renderer::startMainCommandBuffer() {
    auto &perFrameCmds = m_perFrameCommandBuffer[m_currentFrame];

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = m_perFrameCommandPool[m_currentFrame];
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    perFrameCmds.resize(1);
    if (vkAllocateCommandBuffers(m_device, &allocInfo, perFrameCmds.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffer");
    }

    m_currentCommandBuffer = perFrameCmds[0];

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

    if (vkBeginCommandBuffer(m_currentCommandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer");
    }

    vkCmdBindPipeline(m_currentCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineSprites.GetPipeline());
}

void Renderer::recreateSwapChain() {
    int width = 0;
    int height = 0;
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(m_window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(m_device);

    cleanupSwapChain();
    createSwapChain();
    createImageViews();
}

glm::vec4 Renderer::GetClearColor() const {
    return m_clearColor;
}

void Renderer::SetClearColor(const glm::vec4 &clearColor) {
    m_clearColor = clearColor;
    m_pipelineSprites.SetClearColor(clearColor);
}

void Renderer::DrawSprite(float x, float y, float width, float height) {
    std::array<uint16_t, 6> indices {0, 1, 3,  3, 1, 2};
    std::array<Vertex, 4> vertices {{
        {{x, y}, m_currentColor, {m_currentTextureWindow.x0, m_currentTextureWindow.y0}},
        {{x, y + height}, m_currentColor, {m_currentTextureWindow.x0, m_currentTextureWindow.y1}},
        {{x + width, y + height}, m_currentColor, {m_currentTextureWindow.x1, m_currentTextureWindow.y1}},
        {{x + width, y}, m_currentColor, {m_currentTextureWindow.x1, m_currentTextureWindow.y0}},
    }};

    DrawTriangles(indices.data(), indices.size(), vertices.data(), vertices.size());
}

void Renderer::DrawTriangles(const uint16_t *indices, size_t numIndices, const Vertex *vertices, size_t numVertices) {
    if(!m_currentCommandBuffer) {
        throw std::runtime_error("no command buffer");
    }

    if(!m_currentIndexWrite || !m_currentVertexWrite) {
        mapStagingBufferMemory();
    }

    auto vbFull = m_currentVertexWrite + numVertices >= m_vertexWriteEnd;
    auto ibFull = m_currentIndexWrite + numIndices >= m_indexWriteEnd;
    if(vbFull || ibFull) {
        queueCurrentBatch();
        mapStagingBufferMemory();
    }

    if(!m_currentIndexWrite || !m_currentVertexWrite) {
        throw std::runtime_error("write destination is null");
    }

    auto base = static_cast<uint16_t>(m_currentVertexWrite - m_vertexWriteStart);

    // Both indices and vertices need adjustment while being copied to
    // the index/vertex buffers. The indices need the base added,
    // and the vertices need to be colored, for example.
    auto vertexRead = vertices;
    for(int i = 0; i < numVertices; ++i) {
        m_currentVertexWrite->pos = vertexRead->pos;
        m_currentVertexWrite->color = vertexRead->color * m_currentColor;
        m_currentVertexWrite->texCoord = vertexRead->texCoord;
        m_currentVertexWrite->blendMode = m_currentBlendMode;

        m_currentVertexWrite++;
        vertexRead++;
    }

    auto indexRead = indices;
    for(int i = 0; i < numIndices; ++i) {
        *m_currentIndexWrite = *indexRead + base;
        m_currentIndexWrite++;
        indexRead++;
    }

    m_numIndices += numIndices;
    m_numVertices += numVertices;
}

bool Renderer::ReserveTriangles(size_t numIndices, size_t numVertices, uint16_t *&indices, Vertex *&vertices,
                                u_int16_t &baseIndex) {
    if(!m_currentCommandBuffer) {
        throw std::runtime_error("no command buffer");
    }

    if(!m_currentIndexWrite || !m_currentVertexWrite) {
        mapStagingBufferMemory();
    }

    auto vbFull = m_currentVertexWrite + numVertices >= m_vertexWriteEnd;
    auto ibFull = m_currentIndexWrite + numIndices >= m_indexWriteEnd;
    if(vbFull || ibFull) {
        queueCurrentBatch();
        mapStagingBufferMemory();
    }

    if(!m_currentIndexWrite || !m_currentVertexWrite) {
        throw std::runtime_error("write destination is null");
    }

    indices = m_currentIndexWrite;
    vertices = m_currentVertexWrite;
    baseIndex = static_cast<uint16_t>(m_currentVertexWrite - m_vertexWriteStart);

    return true;
}

void Renderer::CommitTriangles(size_t numIndices, size_t numVertices) {
    m_currentVertexWrite += numVertices;
    m_numVertices += numVertices;
    m_currentIndexWrite += numIndices;
    m_numIndices += numIndices;
}

void Renderer::createIndexAndVertexBuffers() {
    m_indexBuffers.resize(getMaxFramesInFlight());
    m_indexStagingBuffers.resize(getMaxFramesInFlight());
    m_vertexBuffers.resize(getMaxFramesInFlight());
    m_vertexStagingBuffers.resize(getMaxFramesInFlight());
}

VkDeviceSize Renderer::getVertexBufferSize() { return getMaxNumVertices() * sizeof(Vertex); }

VkDeviceSize Renderer::getIndexBufferSize() { return getMaxNumIndices() * sizeof(uint16_t); }

void Renderer::cleanupIndexAndVertexBuffers() {
    for(uint32_t i = 0; i < getMaxFramesInFlight(); ++i) {
        cleanupBuffers(m_indexStagingBuffers[i]);
        cleanupBuffers(m_indexBuffers[i]);
        cleanupBuffers(m_vertexStagingBuffers[i]);
        cleanupBuffers(m_vertexBuffers[i]);
    }
}

void Renderer::cleanupBuffers(const std::vector<BoundBuffer> &buffers) {
    for(auto b: buffers) {
        DestroyBuffer(b);
    }
}

VkDeviceSize Renderer::getMaxNumIndices() {
    return 64*1024;
}

VkDeviceSize Renderer::getMaxNumVertices() {
    return 64*1024;
}

VkDeviceSize Renderer::GetNumIndices() {
    return m_currentIndexWrite - m_indexWriteStart;
}

VkDeviceSize Renderer::GetNumVertices() {
    return m_currentVertexWrite - m_vertexWriteStart;
}

void Renderer::SetTexture(std::shared_ptr<ITexture> texture) {
    if(!texture) {
        texture = m_defaultTexture;
    }

    if(texture->GetDescriptorSet() != m_currentDescriptorSet) {
        queueDrawCommand();
        m_currentDescriptorSet = texture->GetDescriptorSet();
    }
    m_currentTextureWindow = texture->GetTextureWindow();
}

bool Renderer::queueDrawCommand() {
    if(m_numIndices > 0) {
        m_drawCommands.emplace_back(m_currentDescriptorSet, m_indexOffset, m_numIndices, m_currentBufferIndex);
        m_indexOffset += m_numIndices;
        m_numIndices = 0;
        m_vertexOffset += m_numVertices;
        m_numVertices = 0;

        return true;
    }

    return false;
}

unsigned long Renderer::GetNumDrawCommands() {
    return m_numDrawCommands;
}

void Renderer::SetColor(const glm::vec4 &color) {
    m_currentColor = color;
}

DebugMessenger *Renderer::GetDebugMessenger() {
    return m_debugMessenger.get();
}

void Renderer::DestroyBufferLater(BoundBuffer buffer) {
    m_buffersToDestroyLater[m_currentFrame].emplace_back(buffer);
}

void Renderer::SetBlendMode(BlendMode bm)
{
    m_currentBlendMode = bm;
}

BlendMode Renderer::GetBlendMode() const
{
    return m_currentBlendMode;
}

BoundBuffer Renderer::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) {
    return BoundBuffer::Create(m_device, m_physicalDevice, size, usage, properties);
}

VkDescriptorSet Renderer::CreateTextureDescriptorSet(VkImageView imageView, VkSampler sampler) {
    return m_pipelineSprites.CreateTextureDescriptorSet(imageView, sampler);
}


#pragma clang diagnostic pop
