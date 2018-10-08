//
// Created by snorri on 8.10.2018.
//

#include "AtlasTexture.h"
#include "TextureAtlas.h"

AtlasTexture::AtlasTexture(TextureAtlas* owner, Area* area) : m_owner(owner), m_area(area) {

}

VkDescriptorSet AtlasTexture::GetDescriptorSet() {
    return m_owner->GetDescriptorSet();
}

TextureWindow AtlasTexture::GetTextureWindow() {
    float x0 = float(m_area->x) / float(m_owner->GetWidth());
    float y0 = float(m_area->y) / float(m_owner->GetHeight());
    float x1 = x0 + float(m_area->width) / float(m_owner->GetWidth());
    float y1 = y0 + float(m_area->height) / float(m_owner->GetHeight());
    return TextureWindow{x0, y0, x1, y1};
}
