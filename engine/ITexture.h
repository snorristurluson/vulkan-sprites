//
// Created by snorri on 8.10.2018.
//

#ifndef VULKAN_SPRITES_ITEXTURE_H
#define VULKAN_SPRITES_ITEXTURE_H

#include <vulkan/vulkan.h>

struct TextureWindow {
    float x0;
    float y0;
    float x1;
    float y1;
};

struct ITexture {
    virtual VkDescriptorSet GetDescriptorSet() = 0;
    virtual TextureWindow GetTextureWindow() = 0;
};

#endif //VULKAN_SPRITES_ITEXTURE_H
