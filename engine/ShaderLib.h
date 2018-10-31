//
// Created by snorri on 26.8.2018.
//

#ifndef VULKAN_SPRITES_SHADERLIB_H
#define VULKAN_SPRITES_SHADERLIB_H


#include <cstdint>
#include <cstddef>
#include <vulkan/vulkan.h>
#include <string>

class ShaderLib {
public:
    static uint8_t* GetVertexShader();
    static size_t GetVertexShaderSize();

    static uint8_t* GetPixelShader();
    static size_t GetPixelShaderSize();
};

VkShaderModule CreateShaderModule(const std::string& name);

#endif //VULKAN_SPRITES_SHADERLIB_H
