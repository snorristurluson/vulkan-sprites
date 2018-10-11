#include <utility>

//
// Created by snorri on 10.10.2018.
//

#include "Font.h"

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
