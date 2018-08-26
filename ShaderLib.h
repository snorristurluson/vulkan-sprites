//
// Created by snorri on 26.8.2018.
//

#ifndef VULKAN_SPRITES_SHADERLIB_H
#define VULKAN_SPRITES_SHADERLIB_H


#include <cstdint>
#include <cstddef>

class ShaderLib {
public:
    static uint8_t* GetVertexShader();
    static size_t GetVertexShaderSize();

    static uint8_t* GetPixelShader();
    static size_t GetPixelShaderSize();
};


#endif //VULKAN_SPRITES_SHADERLIB_H
