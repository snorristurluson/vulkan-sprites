//
// Created by snorri on 25.8.2018.
//

#include <stdexcept>
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(VkDevice device, const std::string& filename)
{
    stbi_uc* pixels = stbi_load(filename.c_str(), &m_width, &m_height, &m_channels, STBI_rgb_alpha);
    if(!pixels) {
        throw std::runtime_error("failed to load texture image");
    }

    VkDeviceSize imageSize = m_width * m_height * 4;
}
