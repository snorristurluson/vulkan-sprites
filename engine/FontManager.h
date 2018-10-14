//
// Created by snorri on 10.10.2018.
//

#ifndef VULKAN_SPRITES_FONTMANAGER_H
#define VULKAN_SPRITES_FONTMANAGER_H

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Font.h"
#include <memory>
#include <string>

class TextureAtlas;

class FontManager {
public:
    explicit FontManager(std::shared_ptr<TextureAtlas> ta);

    std::shared_ptr<Font> GetFont(const std::string& fontname, int pt);

protected:
    FT_Library m_library;
    std::shared_ptr<TextureAtlas> m_textureAtlas;
};


#endif //VULKAN_SPRITES_FONTMANAGER_H
