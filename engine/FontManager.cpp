//
// Created by snorri on 10.10.2018.
//

#include "FontManager.h"

FontManager::FontManager(std::shared_ptr<TextureAtlas> ta) : m_textureAtlas(ta) {
    auto error = FT_Init_FreeType(&m_library);
    if(error) {
        throw std::runtime_error("couldn't initialize FreeType library");
    }
}

std::shared_ptr<Font> FontManager::GetFont(const std::string& fontname, int pt) {
    FT_Face face;
    auto error = FT_New_Face(m_library, fontname.c_str(), 0, &face);
    if(!error) {
        error = FT_Set_Pixel_Sizes(face, 0, pt);
        if(error) {
            throw std::runtime_error("couldn't set pixel sizes");
        }
        return std::make_shared<Font>(face);
    }

    throw std::runtime_error("couldn't load font");
}
