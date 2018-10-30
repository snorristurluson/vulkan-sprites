//
// Created by snorri on 30.10.2018.
//

#ifndef VULKAN_SPRITES_PHYSICALDEVICE_H
#define VULKAN_SPRITES_PHYSICALDEVICE_H

#include <vulkan/vulkan.h>

uint32_t FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typefilter, VkFlags properties);

#endif //VULKAN_SPRITES_PHYSICALDEVICE_H
