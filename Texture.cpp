//
// Created by snorri on 25.8.2018.
//

#include <stdexcept>
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(Renderer *renderer, const std::string &filename)
{
    stbi_uc* pixels = stbi_load(filename.c_str(), &m_width, &m_height, &m_channels, STBI_rgb_alpha);
    if(!pixels) {
        throw std::runtime_error("failed to load texture image");
    }

    VkDeviceSize imageSize = m_width * m_height * 4;
    auto stagingBuffer = renderer->CreateBuffer(
            imageSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    renderer->CopyToBufferMemory(stagingBuffer.bufferMemory, pixels, imageSize);

    stbi_image_free(pixels);

    VkFormat format = VK_FORMAT_R8G8B8A8_UNORM;
    m_boundImage = renderer->CreateImage(
            m_width, m_height,
            format, VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    renderer->TransitionImageLayout(
            m_boundImage.image,
            format,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    renderer->CopyBufferToImage(stagingBuffer.buffer, m_boundImage.image, m_width, m_height);
    renderer->TransitionImageLayout(
            m_boundImage.image,
            format,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    
    renderer->DestroyBuffer(stagingBuffer);
    
    m_imageView = renderer->CreateImageView(m_boundImage.image, format);
    m_sampler = renderer->CreateSampler();
    m_descriptorSet = renderer->CreateTextureDescriptorSet(m_imageView, m_sampler);
}

int Texture::GetWidth() const {
    return m_width;
}

int Texture::GetHeight() const {
    return m_height;
}
