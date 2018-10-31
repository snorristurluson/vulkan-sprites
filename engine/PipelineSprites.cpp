//
// Created by snorri on 30.10.2018.
//

#include "PipelineSprites.h"
#include "ShaderLib.h"
#include "Vertex.h"
#include <stdexcept>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-stack-address"

#define MAX_NUM_TEXTURES 256

struct UniformBufferObject {
    glm::vec2 extent;
};

void PipelineSprites::Initialize(VkDevice device, VkPhysicalDevice physicalDevice, VkExtent2D vpExtent,
                                 VkSwapchainKHR swapchain, VkFormat format) {
    m_device = device;
    m_physicalDevice = physicalDevice;
    m_frameBufferExtent = vpExtent;
    m_swapChain = swapchain;
    m_frameBufferImageFormat = format;

    vkGetSwapchainImagesKHR(m_device, m_swapChain, &m_imageCount, nullptr);
    m_frameBufferImages.resize(m_imageCount);
    vkGetSwapchainImagesKHR(m_device, m_swapChain, &m_imageCount, m_frameBufferImages.data());


    createUniformBuffers();
    createDescriptorPool();
    createDescriptorSetLayouts();
    createPerFrameDescriptorSets();
    createRenderPass();
    createImageViews();
    createFramebuffers();
    createPipeline();

}

void PipelineSprites::Cleanup() {
    vkDestroyPipeline(m_device, m_graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr);
    vkDestroyDescriptorPool(m_device, m_descriptorPool, nullptr);
    vkDestroyDescriptorSetLayout(m_device, m_perFrameDescriptorSetLayout, nullptr);
    vkDestroyDescriptorSetLayout(m_device, m_perTextureDescriptorSetLayout, nullptr);
    vkDestroyRenderPass(m_device, m_renderPass, nullptr);

    for(auto imageView: m_frameBufferImageViews) {
        vkDestroyImageView(m_device, imageView, nullptr);
    }

    for(auto fb: m_frameBuffers) {
        vkDestroyFramebuffer(m_device, fb, nullptr);
    }

    cleanupUniformBuffers();

}

void PipelineSprites::createPipeline() {
    VkShaderModule vertShaderModule = createShaderModule(ShaderLib::GetVertexShader(),
                                                         ShaderLib::GetVertexShaderSize());
    VkShaderModule fragShaderModule = createShaderModule(ShaderLib::GetPixelShader(), ShaderLib::GetPixelShaderSize());

    VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();

    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) m_frameBufferExtent.width;
    viewport.height = (float) m_frameBufferExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor = {};
    scissor.offset = {0, 0};
    scissor.extent = m_frameBufferExtent;

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_NONE;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
            VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;

    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 1.0f;
    colorBlending.blendConstants[1] = 1.0f;
    colorBlending.blendConstants[2] = 1.0f;
    colorBlending.blendConstants[3] = 1.0f;

    std::array<VkDescriptorSetLayout, 2> layouts = {m_perFrameDescriptorSetLayout, m_perTextureDescriptorSetLayout};
    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = layouts.size();
    pipelineLayoutInfo.pSetLayouts = layouts.data();

    if (vkCreatePipelineLayout(m_device, &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }

    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.layout = m_pipelineLayout;
    pipelineInfo.renderPass = m_renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    if (vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_graphicsPipeline) !=
        VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
    }

    vkDestroyShaderModule(m_device, fragShaderModule, nullptr);
    vkDestroyShaderModule(m_device, vertShaderModule, nullptr);
}

VkShaderModule PipelineSprites::createShaderModule(uint8_t *code, size_t codeSize) {
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = codeSize;
    createInfo.pCode = reinterpret_cast<const uint32_t *>(code);

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(m_device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }

    return shaderModule;
}

void PipelineSprites::createDescriptorSetLayouts() {
    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;

    VkDescriptorSetLayoutBinding pf = {};
    pf.binding = 0;
    pf.descriptorCount = 1;
    pf.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    pf.pImmutableSamplers = nullptr;
    pf.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    layoutInfo.pBindings = &pf;

    if (vkCreateDescriptorSetLayout(m_device, &layoutInfo, nullptr, &m_perFrameDescriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout");
    }

    VkDescriptorSetLayoutBinding po = {};
    po.binding = 0;
    po.descriptorCount = 1;
    po.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    po.pImmutableSamplers = nullptr;
    po.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    layoutInfo.pBindings = &po;

    if (vkCreateDescriptorSetLayout(m_device, &layoutInfo, nullptr, &m_perTextureDescriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout");
    }

}

VkDescriptorSet PipelineSprites::CreateTextureDescriptorSet(VkImageView imageView, VkSampler sampler) {
    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = m_descriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &m_perTextureDescriptorSetLayout;

    VkDescriptorSet descriptorSet;
    if (vkAllocateDescriptorSets(m_device, &allocInfo, &descriptorSet) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate texture descriptor set");
    }

    VkDescriptorImageInfo imageInfo = {};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = imageView;
    imageInfo.sampler = sampler;

    VkWriteDescriptorSet descriptorWrites = {};
    descriptorWrites.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites.dstSet = descriptorSet;
    descriptorWrites.dstBinding = 0;
    descriptorWrites.dstArrayElement = 0;
    descriptorWrites.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrites.descriptorCount = 1;
    descriptorWrites.pImageInfo = &imageInfo;

    vkUpdateDescriptorSets(m_device, 1, &descriptorWrites, 0, nullptr);

    return descriptorSet;
}

void PipelineSprites::createPerFrameDescriptorSets() {
    std::vector<VkDescriptorSetLayout> layouts(getMaxFramesInFlight(), m_perFrameDescriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = m_descriptorPool;
    allocInfo.descriptorSetCount = getMaxFramesInFlight();
    allocInfo.pSetLayouts = layouts.data();

    m_perFrameDescriptorSets.resize(getMaxFramesInFlight());
    if (vkAllocateDescriptorSets(m_device, &allocInfo, &m_perFrameDescriptorSets[0]) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets");
    }

    for (size_t i = 0; i < getMaxFramesInFlight(); ++i) {
        VkDescriptorBufferInfo bufferInfo = {};
        bufferInfo.buffer = m_uniformBuffers[i].buffer;
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        VkWriteDescriptorSet descriptorWrites = {};
        descriptorWrites.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites.dstSet = m_perFrameDescriptorSets[i];
        descriptorWrites.dstBinding = 0;
        descriptorWrites.dstArrayElement = 0;
        descriptorWrites.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites.descriptorCount = 1;
        descriptorWrites.pBufferInfo = &bufferInfo;

        vkUpdateDescriptorSets(m_device, 1, &descriptorWrites, 0, nullptr);
    }
}

void PipelineSprites::createDescriptorPool() {
    std::array<VkDescriptorPoolSize, 2> poolSizes = {};
    uint32_t descriptorCount = getMaxFramesInFlight() * 2;

    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = descriptorCount;

    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = MAX_NUM_TEXTURES;

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = poolSizes.size();
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = descriptorCount + MAX_NUM_TEXTURES;

    if (vkCreateDescriptorPool(m_device, &poolInfo, nullptr, &m_descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool");
    }
}

void PipelineSprites::createRenderPass() {
    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = m_frameBufferImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkSubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(m_device, &renderPassInfo, nullptr, &m_renderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass");
    }
}

VkPipeline PipelineSprites::GetPipeline() {return m_graphicsPipeline; }

void PipelineSprites::createFramebuffers() {
    m_frameBuffers.resize(m_frameBufferImageViews.size());

    for (size_t i = 0; i < m_frameBufferImageViews.size(); i++) {
        VkImageView attachments[] = {
                m_frameBufferImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = m_renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = m_frameBufferExtent.width;
        framebufferInfo.height = m_frameBufferExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(m_device, &framebufferInfo, nullptr, &m_frameBuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}

void PipelineSprites::createUniformBuffers() {
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    m_uniformBuffers.resize(getMaxFramesInFlight());

    for (size_t i = 0; i < getMaxFramesInFlight(); ++i) {
        m_uniformBuffers[i] = BoundBuffer::Create(
                m_device,
                m_physicalDevice,
                bufferSize,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    }

}

void PipelineSprites::cleanupUniformBuffers() {
    for (size_t i = 0; i < getMaxFramesInFlight(); ++i) {
        vkDestroyBuffer(m_device, m_uniformBuffers[i].buffer, nullptr);
        vkFreeMemory(m_device, m_uniformBuffers[i].bufferMemory, nullptr);
    }
}

void PipelineSprites::updateUniformBuffer(int frameIx) {
    UniformBufferObject ubo = {};
    ubo.extent.x = m_frameBufferExtent.width;
    ubo.extent.y = m_frameBufferExtent.height;

    void* data;
    vkMapMemory(m_device, m_uniformBuffers[frameIx].bufferMemory, 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(m_device, m_uniformBuffers[frameIx].bufferMemory);
}

void PipelineSprites::BeginRenderPass(VkCommandBuffer cmdBuffer, int frameIx) {
    updateUniformBuffer(frameIx);

    VkRenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_renderPass;
    renderPassInfo.framebuffer = m_frameBuffers[frameIx];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = m_frameBufferExtent;
    renderPassInfo.clearValueCount = 1;
    VkClearValue clearValue = {m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a};
    renderPassInfo.pClearValues = &clearValue;

    vkCmdBeginRenderPass(cmdBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void PipelineSprites::EndRenderPass(VkCommandBuffer cmdBuffer) {
    vkCmdEndRenderPass(cmdBuffer);
}

uint32_t PipelineSprites::getMaxFramesInFlight() {
    return m_imageCount;
}

VkImageView PipelineSprites::createImageView(VkImage image, VkFormat format) {
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

void PipelineSprites::createImageViews() {
    m_frameBufferImageViews.resize(getMaxFramesInFlight());

    for (size_t i = 0; i < getMaxFramesInFlight(); i++) {
        m_frameBufferImageViews[i] = createImageView(m_frameBufferImages[i], m_frameBufferImageFormat);
    }
}

void PipelineSprites::SetClearColor(const glm::vec4 &clearColor) {
    m_clearColor = clearColor;
}

#pragma clang diagnostic pop