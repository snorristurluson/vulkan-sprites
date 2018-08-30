//
// Created by snorri on 25.8.2018.
//

#ifndef VULKAN_SPRITES_TEXTURE_H
#define VULKAN_SPRITES_TEXTURE_H


#include <vulkan/vulkan.h>
#include <string>
#include "Renderer.h"

class Texture {
public:
    Texture(Renderer *renderer, const std::string &filename);

    int GetWidth() const;
    int GetHeight() const;

protected:
    VkImageView m_imageView;
    VkSampler m_sampler;
    int m_width;
    int m_height;
    int m_channels;
    Renderer::BoundImage m_boundImage;
};


#endif //VULKAN_SPRITES_TEXTURE_H
