//
// Created by snorri on 18.9.2018.
//

#ifndef VULKAN_SPRITES_DEBUGMESSENGER_H
#define VULKAN_SPRITES_DEBUGMESSENGER_H

#include <vulkan/vulkan.h>

class DebugMessenger {
public:
    DebugMessenger();

    int GetErrorAndWarningCount();
    int GetErrorCount();
    int GetWarningCount();
    int GetInfoCount();

    static VKAPI_ATTR VkBool32 VKAPI_CALL
    Log(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
                  const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData);
protected:
    int m_errorCount;
    int m_warningCount;
    int m_infoCount;

    VkBool32
    Log(const VkDebugUtilsMessageSeverityFlagBitsEXT &messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData);
};


#endif //VULKAN_SPRITES_DEBUGMESSENGER_H
