//
// Created by snorri on 26.8.2018.
//

#ifndef VULKAN_SPRITES_SHADERLIB_H
#define VULKAN_SPRITES_SHADERLIB_H


#include <cstdint>
#include <cstddef>
#include <vulkan/vulkan.h>
#include <string>

VkShaderModule CreateShaderModule(VkDevice device, const std::string &name);

#endif //VULKAN_SPRITES_SHADERLIB_H
