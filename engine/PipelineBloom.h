//
// Created by snorri on 31.10.2018.
//

#ifndef VULKAN_SPRITES_PIPELINEBLOOM_H
#define VULKAN_SPRITES_PIPELINEBLOOM_H


#include <vulkan/vulkan.h>
#include <vector>
#include "BoundBuffer.h"

class PipelineBloom {
public:

    void Initialize(VkDevice device, VkPhysicalDevice physicalDevice, VkExtent2D vpExtent, std::vector<VkImage> inputImages,
                        VkFormat inputFormat, std::vector<VkImage> outputImages, VkFormat outputFormat);
    void Cleanup();

    void Bloom(uint32_t currentFrameIx, uint32_t nextFrameIx, VkCommandBuffer cmdBuffer);

protected:
    VkDevice m_device;
    VkPhysicalDevice m_physicalDevice;
    uint32_t m_imageCount;
    VkPipeline m_graphicsPipeline;
    VkPipelineLayout m_pipelineLayout;
    VkDescriptorPool m_descriptorPool;
    std::vector<VkDescriptorSet> m_descriptorSets;
    VkDescriptorSetLayout m_descriptorSetLayout;
    VkRenderPass m_renderPass;
    VkFormat m_frameBufferImageFormat;
    VkExtent2D m_frameBufferExtent;
    std::vector<VkFramebuffer> m_frameBuffers;
    std::vector<VkImage> m_frameBufferImages;
    std::vector<VkImageView> m_frameBufferImageViews;
    VkFormat m_inputImageFormat;
    std::vector<VkImage> m_inputImages;
    std::vector<VkImageView> m_inputImageViews;

    std::vector<BoundBuffer> m_uniformBuffers;

    VkSampler m_sampler;

    void createUniformBuffers();

    void createDescriptorPool();

    void createDescriptorSetLayouts();

    void createDescriptorSets();

    void createRenderPass();

    void createImageViews();

    void createFramebuffers();

    void createPipeline();

    void createSampler();

    void createInputImageViews();
};


#endif //VULKAN_SPRITES_PIPELINEBLOOM_H
