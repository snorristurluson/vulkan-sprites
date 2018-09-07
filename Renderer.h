//
// Created by Snorri Sturluson on 20/08/2018.
//

#ifndef VULKAN_SPRITES_RENDERER_H
#define VULKAN_SPRITES_RENDERER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <memory>

class Texture;
class Renderer
{
public:
    enum ValidationState {
        DISABLE_VALIDATION,
        ENABLE_VALIDATION
    };

    struct BoundBuffer {
        VkBuffer buffer;
        VkDeviceMemory bufferMemory;
    };

    struct BoundImage {
        VkImage image;
        VkDeviceMemory imageMemory;
    };

    ~Renderer();

    bool IsInitialized();

    void Initialize(GLFWwindow *window, Renderer::ValidationState validation);

    std::shared_ptr<Texture> CreateTexture(const std::string &filename);

    BoundBuffer CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
    BoundImage CreateImage(
            uint32_t width, uint32_t height,
            VkFormat format, VkImageTiling tiling,
            VkImageUsageFlags usage, VkMemoryPropertyFlags properties);

    void CopyToBufferMemory(VkDeviceMemory bufferMemory, uint8_t *data, VkDeviceSize size);
    void TransitionImageLayout(VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout);

    void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

    void DestroyBuffer(BoundBuffer buffer);

    VkImageView CreateImageView(VkImage image, VkFormat format);

    VkSampler CreateSampler();

    VkDescriptorSet CreateTextureDescriptorSet(VkImageView imageView, VkSampler sampler);

    void DestroyImageView(VkImageView imageView);

    void DestroySampler(VkSampler sampler);

    void DestroyTextureDescriptorSet(VkDescriptorSet descriptorSet);

    void DestroyImage(BoundImage image);

    void WaitUntilDeviceIdle();

    bool StartFrame();

    void EndFrame();

    void DrawSprite(int x, int y, uint32_t width, uint32_t height);

protected:
    GLFWwindow *m_window;

    bool m_isInitialized = false;

    VkInstance m_instance;
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    VkDevice m_device;
    VkSurfaceKHR m_surface;

    VkQueue m_graphicsQueue;
    VkQueue m_presentQueue;

    VkSwapchainKHR m_swapChain;
    std::vector<VkImage> m_swapChainImages;
    VkFormat m_swapChainImageFormat;
    VkExtent2D m_swapChainExtent;
    std::vector<VkImageView> m_swapChainImageViews;
    std::vector<VkFramebuffer> m_swapChainFramebuffers;

    bool m_enableValidationLayers;
    VkDebugUtilsMessengerEXT m_callback;

    VkRenderPass m_renderPass;

    VkDescriptorSetLayout m_perFrameDescriptorSetLayout;
    VkDescriptorSetLayout m_perTextureDescriptorSetLayout;

    VkPipelineLayout m_pipelineLayout;
    VkPipeline m_graphicsPipeline;

    VkCommandPool m_commandPool;
    std::vector<VkCommandPool> m_perFrameCommandPool;
    std::vector<std::vector<VkCommandBuffer>> m_perFrameCommandBuffer;

    VkDescriptorPool m_descriptorPool;
    std::vector<VkDescriptorSet> m_perFrameDescriptorSets;

    std::vector<BoundBuffer> m_uniformBuffers;

    std::vector<VkSemaphore> m_imageAvailableSemaphores;
    std::vector<VkSemaphore> m_renderFinishedSemaphores;
    std::vector<VkFence> m_inFlightFences;
    uint32_t m_currentFrame = 1;
    uint32_t m_nextFrame;

    VkClearValue m_clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
public:
    const VkClearValue &ClearColor() const;

    void SetClearColor(const VkClearValue &m_clearColor);


protected:
    void createInstance();
    void setupDebugCallback();
    void setSurfaceFromWindow(GLFWwindow *window);

    struct QueueFamilyIndices
    {
        uint32_t graphicsFamily = std::numeric_limits<uint32_t>::max();
        uint32_t presentFamily = std::numeric_limits<uint32_t>::max();

        bool isComplete()
        {
            return graphicsFamily != std::numeric_limits<uint32_t>::max() &&
                presentFamily != std::numeric_limits<uint32_t>::max();
        }
    };

    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    bool isDeviceSuitable(VkPhysicalDevice device);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    void pickPhysicalDevice();
    void createLogicalDevice();

    void createSwapChain();

    void createImageViews();

    uint32_t getMaxFramesInFlight();

    VkImageView createImageView(VkImage image, VkFormat format);

    void createRenderPass();

    void createDescriptorSetLayouts();

    void createGraphicsPipeline();

    VkShaderModule createShaderModule(uint8_t* code, size_t codeSize);

    void createFramebuffers();

    void createCommandPool();

    uint32_t findMemoryType(uint32_t bits, VkMemoryPropertyFlags properties);

    VkCommandBuffer beginSingleTimeCommands();

    void endSingleTimeCommands(VkCommandBuffer commandBuffer);

    void createDescriptorPool();

    void createUniformBuffers();

    void createPerFrameDescriptorSets();

    void createSyncObjects();

    void cleanup();

    void cleanupSwapChain();

    void cleanupSyncObjects();

    void cleanupUniformBuffers();

    void cleanupCommandPools();

    void recreateSwapChain();
};


#endif //VULKAN_SPRITES_RENDERER_H
