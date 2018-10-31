//
// Created by snorri on 26.8.2018.
//

#include "ShaderLib.h"
#include <map>

//#define ADD_SHADER(name, filename) \
//    uint8_t s_##name[] = { \
//        #include filename \
//    }; \
//    Initializer s_##name_init(#name, s_##name, sizeof(s_##name));

namespace {
    struct ShaderBinary {
        uint8_t* data;
        size_t size;
    };
    std::map<std::string, ShaderBinary> s_shaders;
    class Initializer {
    public:
        Initializer(const std::string& name, uint8_t* data, size_t size) {
            s_shaders[name] = {data, size};
        }
    };

    uint8_t s_vertexShader[] = {
        #include "../engine/shaders/shader.vert.array"
    };
    Initializer s("spriteVertex", s_vertexShader, sizeof(s_vertexShader));

    uint8_t s_pixelShader[] = {
        #include "../engine/shaders/shader.frag.array"
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

VkShaderModule CreateShaderModule(const std::string &name) {
    return nullptr;
}
