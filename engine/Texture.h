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
    Texture(Renderer *renderer, int width, int height, uint8_t *pixels);

    ~Texture();

    int GetWidth() const;
    int GetHeight() const;
    VkDescriptorSet GetDescriptorSet() override;

protected:
    Renderer* m_renderer;
    VkImageView m_imageView;
    VkSampler m_sampler;
    int m_width;
    int m_height;
    int m_channels;
    BoundImage m_boundImage;
    VkDescriptorSet m_descriptorSet;

    void init(int width, int height, uint8_t *pixels);
};


#endif //VULKAN_SPRITES_TEXTURE_H
