//
// Created by Snorri Sturluson on 20/08/2018.
//

#ifndef VULKAN_SPRITES_RENDERER_H
#define VULKAN_SPRITES_RENDERER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Renderer
{
public:
    Renderer();

    bool checkValidationLayerSupport();

    void createInstance(bool enableValidationLayers);
    void setupDebugCallback();


protected:
    VkInstance m_instance;
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    VkDevice m_device;
    VkDebugUtilsMessengerEXT m_callback;
};


#endif //VULKAN_SPRITES_RENDERER_H
