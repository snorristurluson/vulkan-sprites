//
// Created by snorri on 25.8.2018.
//

#include <stdexcept>
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(Renderer *renderer, const std::string &filename) : m_renderer(renderer)
{
    int width;
    int height;
    stbi_uc* pixels = stbi_load(filename.c_str(), &width, &height, &m_channels, STBI_rgb_alpha);
    if(!pixels) {
        throw std::runtime_error("failed to load texture image");
    }

    init(width, height, pixels);

    stbi_image_free(pixels);
}

Texture::Texture(Renderer *renderer, int width, int height, uint8_t *pixels) : m_renderer(renderer) {
    init(width, height, pixels);
}

void Texture::init(int width, int height, uint8_t *pixels) {
    m_width = width;
    m_height = height;
    VkDeviceSize imageSize = m_width * m_height * 4;
    auto stagingBuffer = m_renderer->CreateBuffer(
            imageSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    m_renderer->CopyToBufferMemory(stagingBuffer.bufferMemory, pixels, imageSize);

    VkFormat format = VK_FORMAT_R8G8B8A8_UNORM;
    m_boundImage = m_renderer->CreateImage(
            m_width, m_height,
            format, VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    m_renderer->TransitionImageLayout(
            m_boundImage.image,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    m_renderer->CopyBufferToImage(stagingBuffer.buffer, m_boundImage.image, m_width, m_height);
    m_renderer->TransitionImageLayout(
            m_boundImage.image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    m_renderer->DestroyBufferLater(stagingBuffer);

    m_imageView = m_renderer->CreateImageView(m_boundImage.image, format);
    m_sampler = m_renderer->CreateSampler();
    m_descriptorSet = m_renderer->CreateTextureDescriptorSet(m_imageView, m_sampler);
}

Texture::~Texture() {
    if(m_imageView) {
        m_renderer->DestroyImageView(m_imageView);
    }
    if(m_sampler) {
        m_renderer->DestroySampler(m_sampler);
    }
    m_renderer->DestroyImage(m_boundImage);
}

int Texture::GetWidth() const {
    return m_width;
}

int Texture::GetHeight() const {
    return m_height;
}

VkDescriptorSet Texture::GetDescriptorSet() {
    return m_descriptorSet;
}

TextureWindow Texture::GetTextureWindow() {
    return TextureWindow{0.0f, 0.0f, 1.0f, 1.0f};
}
