//
// Created by snorri on 25.8.2018.
//

#ifndef VULKAN_SPRITES_TEXTURE_H
#define VULKAN_SPRITES_TEXTURE_H


#include <vulkan/vulkan.h>
#include <string>

class Texture {
public:
    Texture(VkDevice device, const std::string& filename);

protected:
    VkImage m_textureImage;
    VkDeviceMemory m_textureImageMemory;
    VkImageView m_textureImageView;
    VkSampler m_textureSampler;
    int m_width;
    int m_height;
    int m_channels;
};


#endif //VULKAN_SPRITES_TEXTURE_H
