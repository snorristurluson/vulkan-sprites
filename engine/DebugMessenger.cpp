//
// Created by snorri on 18.9.2018.
//

#include "DebugMessenger.h"
#include <iostream>

#define UNUSED(x) (void(x))

VkBool32
DebugMessenger::Log(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
                    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData) {
    auto pThis = static_cast<DebugMessenger*>(pUserData);
    return pThis->Log(messageSeverity, messageType, pCallbackData);

}

VkBool32 DebugMessenger::Log(const VkDebugUtilsMessageSeverityFlagBitsEXT &messageSeverity,
                             VkDebugUtilsMessageTypeFlagsEXT messageType,
                             const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData) {
    UNUSED(messageType);

    std::string severity;
    if(messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
        severity = "info";
        m_infoCount++;
    }
    if(messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        severity = "warning";
        m_warningCount++;
    }
    if(messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
        severity = "error";
        m_errorCount++;
    }

    if (!severity.empty()) {
        std::cerr << severity << ": " << pCallbackData->pMessage << std::endl;
    }

    return VK_FALSE;
}

DebugMessenger::DebugMessenger() : m_errorCount(0), m_warningCount(0), m_infoCount(0) {
}

int DebugMessenger::GetErrorAndWarningCount() {
    return m_errorCount + m_warningCount;
}

int DebugMessenger::GetErrorCount() {
    return m_errorCount;
}

int DebugMessenger::GetWarningCount() {
    return m_warningCount;
}

int DebugMessenger::GetInfoCount() {
    return m_infoCount;
}
