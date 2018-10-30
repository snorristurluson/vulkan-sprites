//
// Created by snorri on 30.10.2018.
//

#ifndef VULKAN_SPRITES_BOUNDBUFFER_H
#define VULKAN_SPRITES_BOUNDBUFFER_H

#include <vulkan/vulkan.h>
#include <stdexcept>

struct BoundBuffer {
    struct VkBuffer_T * buffer;
    struct VkDeviceMemory_T * bufferMemory;

    static BoundBuffer Create(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
};


#endif //VULKAN_SPRITES_BOUNDBUFFER_H
