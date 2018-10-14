//
// Created by snorri on 10.10.2018.
//

#ifndef VULKAN_SPRITES_FONT_H
#define VULKAN_SPRITES_FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include "Glyph.h"

#include <memory>
#include <map>
#include <string>

class TextureAtlas;

struct TextDimensions {
    int width;
    int height;
};

class Renderer;
class Font {
public:
    Font(const FT_Face& face, std::shared_ptr<TextureAtlas> ta);

    int GetNumGlyphs();
    std::shared_ptr<Glyph> GetGlyph(uint16_t c);

    TextDimensions Measure(const std::string& text);
    void Draw(Renderer &r, float x, float y, const std::string &text);

protected:
    FT_Face m_face;
    std::shared_ptr<TextureAtlas> m_textureAtlas;
    std::map<uint16_t, std::shared_ptr<Glyph>> m_glyphs;
};


#endif //VULKAN_SPRITES_FONT_H
