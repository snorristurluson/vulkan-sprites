//
// Created by snorri on 8.10.2018.
//

#ifndef VULKAN_SPRITES_ATLASTEXTURE_H
#define VULKAN_SPRITES_ATLASTEXTURE_H

#include "ITexture.h"

class Area;
class TextureAtlas;

class AtlasTexture : public ITexture {
public:
    explicit AtlasTexture(TextureAtlas* owner, Area* area);

    VkDescriptorSet GetDescriptorSet() override;

    TextureWindow GetTextureWindow() override;

protected:
    TextureAtlas* m_owner;
    Area* m_area;
};


#endif //VULKAN_SPRITES_ATLASTEXTURE_H
