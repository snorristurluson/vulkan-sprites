//
// Created by snorri on 30.10.2018.
//

#ifndef VULKAN_SPRITES_PIPELINESPRITES_H
#define VULKAN_SPRITES_PIPELINESPRITES_H


#include <vulkan/vulkan.h>
#include <stdexcept>
#include <vector>
#include <glm/vec4.hpp>
#include "BoundBuffer.h"

class PipelineSprites {
public:
    void Initialize(VkDevice device, VkPhysicalDevice physicalDevice, VkExtent2D vpExtent,
                    std::vector<VkImage> frameBuffers, VkFormat format);
    void Cleanup();

    VkDescriptorSet CreateTextureDescriptorSet(VkImageView imageView, VkSampler sampler);

    VkPipeline GetPipeline();
    VkPipelineLayout GetPipelineLayout() { return m_pipelineLayout; }
    VkDescriptorSet GetPerFrameDescriptorSet(int frameIx) { return m_perFrameDescriptorSets[frameIx]; }

    void BeginRenderPass(VkCommandBuffer cmdBuffer, int frameIx);

    void EndRenderPass(VkCommandBuffer cmdBuffer);

    void SetClearColor(const glm::vec4 &clearColor);

protected:
    VkDevice m_device;
    VkPhysicalDevice m_physicalDevice;
    uint32_t m_imageCount;
    VkPipeline m_graphicsPipeline;
    VkPipelineLayout m_pipelineLayout;
    VkDescriptorPool m_descriptorPool;
    std::vector<VkDescriptorSet> m_perFrameDescriptorSets;
    VkDescriptorSetLayout m_perFrameDescriptorSetLayout;
    VkDescriptorSetLayout m_perTextureDescriptorSetLayout;
    VkRenderPass m_renderPass;
    VkFormat m_frameBufferImageFormat;
    VkExtent2D m_frameBufferExtent;
    std::vector<VkFramebuffer> m_frameBuffers;
    std::vector<VkImage> m_frameBufferImages;

    std::vector<VkImageView> m_frameBufferImageViews;
    std::vector<BoundBuffer> m_uniformBuffers;

    glm::vec4 m_clearColor;

protected:
    VkShaderModule createShaderModule(uint8_t *code, size_t codeSize);
    void createDescriptorPool();
    void createPerFrameDescriptorSets();
    void createDescriptorSetLayouts();
    void createRenderPass();
    void createFramebuffers();
    VkImageView createImageView(VkImage image, VkFormat format);
    void createImageViews();
    void createUniformBuffers();
    void createPipeline();

    void cleanupUniformBuffers();

    void updateUniformBuffer(int frameIx);

    uint32_t getMaxFramesInFlight();
};


#endif //VULKAN_SPRITES_PIPELINESPRITES_H
