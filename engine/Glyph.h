//
// Created by snorri on 10.10.2018.
//

#ifndef VULKAN_SPRITES_GLYPH_H
#define VULKAN_SPRITES_GLYPH_H

#include <ft2build.h>
#include FT_FREETYPE_H

class Glyph {
public:
    Glyph(FT_Face face, FT_UInt ix);

    int GetLeft();
    int GetTop();

protected:
    FT_Face m_face;
    FT_UInt m_glyphIndex;
    int m_left;
    int m_top;
};


#endif //VULKAN_SPRITES_GLYPH_H
