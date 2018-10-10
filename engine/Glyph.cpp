//
// Created by snorri on 10.10.2018.
//

#include <stdexcept>
#include "Glyph.h"
#include "TextureAtlas.h"

Glyph::Glyph(FT_Face face, FT_UInt ix, std::shared_ptr<TextureAtlas> ta) : m_face(face), m_glyphIndex(ix) {
    auto error = FT_Load_Glyph(m_face, m_glyphIndex, FT_LOAD_DEFAULT);
    if(error) {
        throw std::runtime_error("failed to load glyph");
    }

    FT_GlyphSlot glyphSlot = m_face->glyph;
    error = FT_Render_Glyph(glyphSlot, FT_RENDER_MODE_NORMAL);
    if(error) {
        throw std::runtime_error("failed to render glyph");
    }

    m_left = glyphSlot->bitmap_left;
    m_top = glyphSlot->bitmap_top;

    auto width = glyphSlot->bitmap.width;
    auto height = glyphSlot->bitmap.rows;
    auto bufferSize = width*height*4;
    std::vector<uint8_t> buffer(bufferSize);
    auto mode = glyphSlot->bitmap.pixel_mode;
    if(mode != FT_PIXEL_MODE_GRAY || glyphSlot->bitmap.num_grays != 256) {
        throw std::runtime_error("unsupported pixel mode");
    }

    uint8_t* src = glyphSlot->bitmap.buffer;
    uint8_t* startOfLine = src;
    int dst = 0;

    for(int y = 0; y < height; ++y) {
        src = startOfLine;
        for(int x = 0; x < width; ++x) {
            auto value = *src;
            src++;

            buffer[dst++] = value;
            buffer[dst++] = value;
            buffer[dst++] = value;
            buffer[dst++] = 0xff;
        }
        startOfLine += glyphSlot->bitmap.pitch;
    }
    m_texture = ta->Add(width, height, buffer.data());
}

int Glyph::GetLeft() {
    return m_left;
}

int Glyph::GetTop() {
    return m_top;
}

std::shared_ptr<ITexture> Glyph::GetTexture() {
    return m_texture;
}
