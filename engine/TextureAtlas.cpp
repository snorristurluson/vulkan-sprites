//
// Created by snorri on 7.10.2018.
//

#include "TextureAtlas.h"
#include "Renderer.h"

#include "../extern/stb_image.h"
#include <string>

TextureAtlas::TextureAtlas() : m_width(0), m_height(0), m_renderer(nullptr)
{}

TextureAtlas::~TextureAtlas() {
    m_renderer->DestroyImageView(m_imageView);
    m_renderer->DestroySampler(m_sampler);
    m_renderer->DestroyImage(m_boundImage);
}

void TextureAtlas::Initialize(Renderer *pRenderer, uint32_t width, uint32_t height) {
    m_renderer = pRenderer;

    m_width = width;
    m_height = height;

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
    m_renderer->TransitionImageLayout(
            m_boundImage.image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    m_imageView = m_renderer->CreateImageView(m_boundImage.image, format);
    m_sampler = m_renderer->CreateSampler();
    m_descriptorSet = m_renderer->CreateTextureDescriptorSet(m_imageView, m_sampler);

    m_allocator.Initialize(m_width, m_height);
}

int TextureAtlas::GetWidth() {
    return m_width;
}

int TextureAtlas::GetHeight() {
    return m_height;
}

std::shared_ptr<AtlasTexture> TextureAtlas::Add(const std::string &filename) {
    int width;
    int height;
    int channels;
    stbi_uc* pixels = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);
    if(!pixels) {
        throw std::runtime_error("failed to load texture image");
    }

    auto result = Add(static_cast<uint32_t>(width), static_cast<uint32_t>(height), pixels);

    stbi_image_free(pixels);

    return result;
}

std::shared_ptr<AtlasTexture> TextureAtlas::Add(uint32_t width, uint32_t height, uint8_t *pixels) {
    auto area = m_allocator.Allocate(width, height);
    if(!area) {
        return nullptr;
    }

    VkDeviceSize imageSize = width * height * 4;
    auto stagingBuffer = m_renderer->CreateBuffer(
            imageSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    m_renderer->CopyToBufferMemory(stagingBuffer.bufferMemory, pixels, imageSize);

    m_renderer->TransitionImageLayout(
            m_boundImage.image,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    m_renderer->CopyBufferToImage(stagingBuffer.buffer, m_boundImage.image, width, height, area->x, area->y);
    m_renderer->TransitionImageLayout(
            m_boundImage.image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    m_renderer->DestroyBufferLater(stagingBuffer);


    return std::make_shared<AtlasTexture>(this, area);
}

VkDescriptorSet TextureAtlas::GetDescriptorSet() {
    return m_descriptorSet;
}

TextureWindow TextureAtlas::GetTextureWindow() {
    return TextureWindow{0.0f, 0.0f, 1.0f, 1.0f};
}

