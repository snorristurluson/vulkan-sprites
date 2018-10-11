#include <utility>

//
// Created by snorri on 10.10.2018.
//

#include "Font.h"
#include "Renderer.h"

Font::Font(const FT_Face& face, std::shared_ptr<TextureAtlas> ta) : m_face(face), m_textureAtlas(std::move(ta)) {

}

int Font::GetNumGlyphs() {
    return static_cast<int>(m_face->num_glyphs);
}

std::shared_ptr<Glyph> Font::GetGlyph(uint16_t c) {
    auto foundIt = m_glyphs.find(c);
    if(foundIt != m_glyphs.end()) {
        return foundIt->second;
    }

    auto glyphIndex = FT_Get_Char_Index(m_face, c);
    auto glyph = std::make_shared<Glyph>(m_face, glyphIndex, m_textureAtlas);

    m_glyphs[c] = glyph;
    return glyph;
}

TextDimensions Font::Measure(const std::string& text) {
    int width = 0;
    int height = 0;

    for(auto c: text) {
        auto glyph = GetGlyph(c);
        width += glyph->GetAdvance();
        height = std::max(height, glyph->GetTop() + glyph->GetHeight());
    }

    return TextDimensions{width, height};
}

void Font::Draw(Renderer &r, float x, float y, const std::string &text) {
    int pos = 0;

    for(auto c: text) {
        auto glyph = GetGlyph(c);
        auto texture = glyph->GetTexture();
        if(texture) {
            r.SetTexture(texture);
            float x0 = pos + glyph->GetLeft();
            float y0 = y - glyph->GetTop();
            r.DrawSprite(x0, y0, texture->GetWidth(), texture->GetHeight());
        }
        pos += glyph->GetAdvance();
    }
}
