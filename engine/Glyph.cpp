//
// Created by snorri on 10.10.2018.
//

#include <stdexcept>
#include "Glyph.h"

Glyph::Glyph(FT_Face face, FT_UInt ix) : m_face(face), m_glyphIndex(ix) {
    auto error = FT_Load_Glyph(m_face, m_glyphIndex, FT_LOAD_DEFAULT);
    if(error) {
        throw std::runtime_error("failed to load glyph");
    }

    error = FT_Render_Glyph(m_face->glyph, FT_RENDER_MODE_NORMAL);
    if(error) {
        throw std::runtime_error("failed to render glyph");
    }

    m_left = m_face->glyph->bitmap_left;
    m_top = m_face->glyph->bitmap_top;
}

int Glyph::GetLeft() {
    return m_left;
}

int Glyph::GetTop() {
    return m_top;
}
