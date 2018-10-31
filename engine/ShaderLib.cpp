//
// Created by snorri on 26.8.2018.
//

#include "ShaderLib.h"
#include <map>

#define STRINGIFY_HELPER(x) #x
#define STRINGIFY(x) STRINGIFY_HELPER(x)

#define ADD_SHADER(name) \
    Initializer name##_init(STRINGIFY(name), name, sizeof(name));

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

    uint8_t spriteVertex[] = {
        #include "shaders/sprite.vert.array"
    };
    ADD_SHADER(spriteVertex);

    uint8_t spriteFragment[] = {
        #include "shaders/sprite.frag.array"
    };
    ADD_SHADER(spriteFragment)

    uint8_t bloomVertex[] = {
        #include "shaders/bloom.vert.array"
    };
    ADD_SHADER(bloomVertex);

    uint8_t bloomFragment[] = {
        #include "shaders/bloom.frag.array"
    };
    ADD_SHADER(bloomFragment)
}

VkShaderModule CreateShaderModule(VkDevice device, const std::string &name) {
    auto foundIt = s_shaders.find(name);
    if(foundIt == s_shaders.end()) {
        throw std::runtime_error("unrecognized shader");
    }

    auto code = foundIt->second.data;
    auto codeSize = foundIt->second.size;

    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = codeSize;
    createInfo.pCode = reinterpret_cast<const uint32_t *>(code);

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }

    return shaderModule;
}
