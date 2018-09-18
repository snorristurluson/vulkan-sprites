//
// Created by Snorri Sturluson on 20/08/2018.
//

#ifndef VULKAN_SPRITES_RENDERER_H
#define VULKAN_SPRITES_RENDERER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <memory>
#include <glm/vec4.hpp>
#include "DebugMessenger.h"

class Texture;
class Vertex;

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

    Renderer();
    ~Renderer();

    bool IsInitialized();

    void Initialize(GLFWwindow *window, Renderer::ValidationState validation);

    DebugMessenger* GetDebugMessenger();

    std::shared_ptr<Texture> CreateTexture(const std::string &filename);

    BoundBuffer CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
    void DestroyBuffer(BoundBuffer buffer);

    BoundImage CreateImage(
            uint32_t width, uint32_t height,
            VkFormat format, VkImageTiling tiling,
            VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
    void DestroyImage(BoundImage image);

    void CopyToBufferMemory(VkDeviceMemory bufferMemory, uint8_t *data, VkDeviceSize size);

    void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

    void TransitionImageLayout(VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout);

    VkImageView CreateImageView(VkImage image, VkFormat format);
    void DestroyImageView(VkImageView imageView);

    VkSampler CreateSampler();
    void DestroySampler(VkSampler sampler);

    VkDescriptorSet CreateTextureDescriptorSet(VkImageView imageView, VkSampler sampler);
    void DestroyTextureDescriptorSet(VkDescriptorSet descriptorSet);

    void WaitUntilDeviceIdle();

    bool StartFrame();

    void EndFrame();

    void DrawSprite(float x, float y, float width, float height);

    VkDeviceSize GetNumIndices();
    VkDeviceSize GetNumVertices();

    const VkClearValue &GetClearColor() const;
    void SetClearColor(const VkClearValue &m_clearColor);

    void SetColor(const glm::vec4& color);

    void SetTexture(std::shared_ptr<Texture> texture);

    int GetNumDrawCommands();

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
    std::unique_ptr<DebugMessenger> m_debugMessenger;

    VkRenderPass m_renderPass;

    VkDescriptorSetLayout m_perFrameDescriptorSetLayout;
    VkDescriptorSetLayout m_perTextureDescriptorSetLayout;

    VkPipelineLayout m_pipelineLayout;
    VkPipeline m_graphicsPipeline;

    VkCommandPool m_commandPool;
    std::vector<VkCommandPool> m_perFrameCommandPool;
    std::vector<std::vector<VkCommandBuffer>> m_perFrameCommandBuffer;
    VkCommandBuffer m_currentCommandBuffer;

    VkDescriptorPool m_descriptorPool;
    std::vector<VkDescriptorSet> m_perFrameDescriptorSets;

    std::vector<BoundBuffer> m_uniformBuffers;

    std::vector<VkSemaphore> m_imageAvailableSemaphores;
    std::vector<VkSemaphore> m_renderFinishedSemaphores;
    std::vector<VkFence> m_inFlightFences;
    uint32_t m_currentFrame = 1;
    uint32_t m_nextFrame;

    VkClearValue m_clearColor = {0.0f, 0.0f, 0.0f, 1.0f};

    std::vector<BoundBuffer> m_indexBuffers;
    std::vector<BoundBuffer> m_indexStagingBuffers;
    std::vector<BoundBuffer> m_vertexBuffers;
    std::vector<BoundBuffer> m_vertexStagingBuffers;

    uint16_t* m_indexWriteStart;
    uint16_t* m_currentIndexWrite;
    uint16_t* m_indexWriteEnd;
    Vertex* m_vertexWriteStart;
    Vertex* m_currentVertexWrite;
    Vertex* m_vertexWriteEnd;

    uint16_t m_numIndices;
    uint16_t m_indexOffset;
    uint16_t m_numVertices;
    uint16_t m_vertexOffset;

    std::shared_ptr<Texture> m_defaultTexture;
    std::shared_ptr<Texture> m_currentTexture;

    glm::vec4 m_currentColor;

    struct DrawCommand {
        DrawCommand(std::shared_ptr<Texture> t, uint16_t bi, uint16_t ni) :
            texture(t),
            baseIndex(bi),
            numIndices(ni)
        {}
        std::shared_ptr<Texture> texture;
        uint16_t baseIndex;
        uint16_t numIndices;
    };

    std::vector<DrawCommand> m_drawCommands;
    int m_numDrawCommands;

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

    void createIndexAndVertexBuffers();

    VkDeviceSize getMaxNumIndices();

    VkDeviceSize getMaxNumVertices();

    void cleanupIndexAndVertexBuffers();

    void startMainCommandBuffer();

    VkDeviceSize getIndexBufferSize();

    VkDeviceSize getVertexBufferSize();

    void mapStagingBufferMemory();

    void unmapStagingBuffers();

    void copyStagingBuffersToDevice(VkCommandBuffer commandBuffer) const;

    void updateUniformBuffer() const;

    void queueDrawCommand();
};


#endif //VULKAN_SPRITES_RENDERER_H
