//
// Created by snorri on 8.10.2018.
//

#ifndef VULKAN_SPRITES_ITEXTURE_H
#define VULKAN_SPRITES_ITEXTURE_H

#include <vulkan/vulkan.h>

struct ITexture {
    virtual VkDescriptorSet GetDescriptorSet() = 0;
};

#endif //VULKAN_SPRITES_ITEXTURE_H
