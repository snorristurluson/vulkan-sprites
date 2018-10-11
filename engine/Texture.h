//
// Created by snorri on 25.8.2018.
//

#ifndef VULKAN_SPRITES_TEXTURE_H
#define VULKAN_SPRITES_TEXTURE_H


#include <vulkan/vulkan.h>
#include <string>
#include "Renderer.h"
#include "ITexture.h"

class Texture : public ITexture {
public:
    Texture(Renderer *renderer, const std::string &filename);
    Texture(Renderer *renderer, uint32_t width, uint32_t height, uint8_t *pixels);

    ~Texture();

    VkDescriptorSet GetDescriptorSet() override;
    TextureWindow GetTextureWindow() override;
    int GetWidth() override;
    int GetHeight() override;

protected:
    Renderer* m_renderer;
    VkImageView m_imageView;
    VkSampler m_sampler;
    uint32_t m_width;
    uint32_t m_height;
    BoundImage m_boundImage;
    VkDescriptorSet m_descriptorSet;

    void init(uint32_t width, uint32_t height, uint8_t *pixels);
};


#endif //VULKAN_SPRITES_TEXTURE_H
