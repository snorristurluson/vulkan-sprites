//
// Created by snorri on 8.10.2018.
//

#ifndef VULKAN_SPRITES_ATLASTEXTURE_H
#define VULKAN_SPRITES_ATLASTEXTURE_H

class Area;
class TextureAtlas;

class AtlasTexture {
public:
    explicit AtlasTexture(TextureAtlas* owner, Area* area);

protected:
    TextureAtlas* m_owner;
    Area* m_area;
};


#endif //VULKAN_SPRITES_ATLASTEXTURE_H
