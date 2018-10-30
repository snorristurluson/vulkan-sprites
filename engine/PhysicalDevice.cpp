//
// Created by snorri on 30.10.2018.
//

#include "PhysicalDevice.h"

#include <stdexcept>

uint32_t FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typefilter, VkFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i) {
        if ((typefilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type");
}
