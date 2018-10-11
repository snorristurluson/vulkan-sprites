//
// Created by snorri on 10.10.2018.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <gtest/gtest.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "FontManager.h"
#include "Font.h"

class FontManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        m_window = glfwCreateWindow(800, 600, "FontManagerTest", nullptr, nullptr);
        m_renderer.Initialize(m_window, Renderer::ENABLE_VALIDATION);
        m_textureAtlas = m_renderer.CreateTextureAtlas(256, 256);
    }

    void TearDown() override {
        glfwDestroyWindow(m_window);
    }

    GLFWwindow *m_window{};
    Renderer m_renderer;
    std::shared_ptr<TextureAtlas> m_textureAtlas;
};

TEST_F(FontManagerTest, CanCreate) {
    FontManager fm(m_textureAtlas);

    EXPECT_EQ(m_renderer.GetDebugMessenger()->GetErrorAndWarningCount(), 0);

}

TEST_F(FontManagerTest, GetFont) {
    FontManager fm(m_textureAtlas);

    auto font = fm.GetFont("/usr/share/fonts/truetype/freefont/FreeSerif.ttf", 16);

    ASSERT_NE(font, nullptr);
}

TEST_F(FontManagerTest, Font_GetNumGlyphs) {
    FontManager fm(m_textureAtlas);

    auto font = fm.GetFont("/usr/share/fonts/truetype/freefont/FreeSerif.ttf", 16);
    ASSERT_NE(font, nullptr);

    int n = font->GetNumGlyphs();
    EXPECT_EQ(n, 10538);
}

TEST_F(FontManagerTest, Font_GetGlyph) {
    FontManager fm(m_textureAtlas);

    auto font = fm.GetFont("/usr/share/fonts/truetype/freefont/FreeSerif.ttf", 16);
    ASSERT_NE(font, nullptr);

    auto glyph = font->GetGlyph('a');
    ASSERT_NE(glyph, nullptr);

    auto left = glyph->GetLeft();
    auto top = glyph->GetTop();

    EXPECT_EQ(left, 0);
    EXPECT_EQ(top, 7);

    auto texture = glyph->GetTexture();
    ASSERT_NE(texture, nullptr);

    EXPECT_EQ(texture->GetWidth(), 7);
    EXPECT_EQ(texture->GetHeight(), 7);
    EXPECT_EQ(glyph->GetWidth(), 7);
}

TEST_F(FontManagerTest, Font_GetGlyph_Space) {
    FontManager fm(m_textureAtlas);

    auto font = fm.GetFont("/usr/share/fonts/truetype/freefont/FreeSerif.ttf", 16);
    ASSERT_NE(font, nullptr);

    auto glyph = font->GetGlyph(' ');
    ASSERT_NE(glyph, nullptr);

    auto left = glyph->GetLeft();
    auto top = glyph->GetTop();

    EXPECT_EQ(left, 0);
    EXPECT_EQ(top, 0);

    auto texture = glyph->GetTexture();
    EXPECT_EQ(texture, nullptr);

    EXPECT_EQ(glyph->GetAdvance(), 4);
}

TEST_F(FontManagerTest, Font_Measure) {
    FontManager fm(m_textureAtlas);

    auto font = fm.GetFont("/usr/share/fonts/truetype/freefont/FreeSerif.ttf", 16);
    auto m = font->Measure("This is a test");

    EXPECT_EQ(m.width, 80);
    EXPECT_EQ(m.height, 24);
}

TEST_F(FontManagerTest, Font_Render) {
    FontManager fm(m_textureAtlas);

    auto font = fm.GetFont("/usr/share/fonts/truetype/freefont/FreeSerif.ttf", 16);

    m_renderer.StartFrame();
    font->Draw(m_renderer, 0, 128, "This is a test");
    m_renderer.EndFrame();
    m_renderer.WaitUntilDeviceIdle();

    EXPECT_EQ(m_renderer.GetDebugMessenger()->GetErrorAndWarningCount(), 0);
}

#pragma clang diagnostic pop
