//
// Created by snorri on 26.8.2018.
//

#include "ShaderLib.h"

namespace {
    uint8_t s_vertexShader[] = {
        #include "shaders/shader.vert.array"
    };
    uint8_t s_pixelShader[] = {
        #include "shaders/shader.frag.array"
    };
}

uint8_t *ShaderLib::GetVertexShader() {
    return s_vertexShader;
}

size_t ShaderLib::GetVertexShaderSize() {
    return sizeof(s_vertexShader);
}

uint8_t *ShaderLib::GetPixelShader() {
    return s_pixelShader;
}

size_t ShaderLib::GetPixelShaderSize() {
    return sizeof(s_pixelShader);
}
