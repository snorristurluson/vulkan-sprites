//
// Created by snorri on 7.10.2018.
//

#ifndef VULKAN_SPRITES_TEXTUREATLAS_H
#define VULKAN_SPRITES_TEXTUREATLAS_H

#include "Renderer.h"
#include "ITexture.h"
#include "AreaAllocator.h"
#include "AtlasTexture.h"

class TextureAtlas : public ITexture {

public:
    ~TextureAtlas();

    void Initialize(Renderer *pRenderer, int width, int height);

    std::shared_ptr<AtlasTexture> Add(const std::string & filename);

    std::shared_ptr<AtlasTexture> Add(int width, int height, uint8_t* pixels);

    VkDescriptorSet GetDescriptorSet() override;
    TextureWindow GetTextureWindow() override;
    int GetWidth() override;
    int GetHeight() override;

protected:
    int m_width;
    int m_height;
    Renderer *m_renderer;
    BoundImage m_boundImage;
    VkImageView m_imageView;
    VkSampler m_sampler;
    VkDescriptorSet m_descriptorSet;
    AreaAllocator m_allocator;

protected:
};


#endif //VULKAN_SPRITES_TEXTUREATLAS_H
