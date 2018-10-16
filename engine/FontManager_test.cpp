//
// Created by snorri on 10.10.2018.
//

#include "catch.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "FontManager.h"
#include "Font.h"

static const char *const FONTNAME = "resources/montserrat/Montserrat-Bold.ttf";

TEST_CASE("FontManager") {

    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    auto window = glfwCreateWindow(800, 600, "FontManagerTest", nullptr, nullptr);

    Renderer r;
    r.Initialize(window, Renderer::ENABLE_VALIDATION);
    auto textureAtlas = r.CreateTextureAtlas(256, 256);

    SECTION("constructor") {
        FontManager fm(textureAtlas);
        REQUIRE(r.GetDebugMessenger()->GetErrorAndWarningCount() == 0);
    }

    SECTION("GetFont") {
        FontManager fm(textureAtlas);

        auto font = fm.GetFont(FONTNAME, 16);
        REQUIRE(font);
    }

    SECTION("Font") {
        FontManager fm(textureAtlas);
        auto font = fm.GetFont(FONTNAME, 16);

        SECTION("GetNumGlyphs") {
            auto n = font->GetNumGlyphs();
            REQUIRE(n == 263);
        }

        SECTION("GetGlyph") {
            SECTION("regular") {
                auto glyph = font->GetGlyph('a');
                REQUIRE(glyph);

                SECTION("GetLeft") {
                    REQUIRE(glyph->GetLeft() == 0);
                }

                SECTION("GetTop") {
                    REQUIRE(glyph->GetTop() == 9);
                }

                SECTION("GetWidth") {
                    REQUIRE(glyph->GetWidth() == 9);
                }

                SECTION("GetTexture") {
                    auto tex = glyph->GetTexture();
                    REQUIRE(tex);
                    REQUIRE(tex->GetWidth() == 9);
                    REQUIRE(tex->GetHeight() == 10);
                }
            }

            SECTION("space") {
                auto glyph = font->GetGlyph(' ');
                REQUIRE(glyph);

                SECTION("GetLeft") {
                    REQUIRE(glyph->GetLeft() == 0);
                }

                SECTION("GetTop") {
                    REQUIRE(glyph->GetTop() == 0);
                }

                SECTION("GetWidth") {
                    REQUIRE(glyph->GetWidth() == 0);
                }

                SECTION("GetTexture") {
                    auto tex = glyph->GetTexture();
                    REQUIRE(!tex);
                }
            }
        }

        SECTION("Measure") {
            auto m = font->Measure("This is a test");
            REQUIRE(m.width == 100);
            REQUIRE(m.height == 26);
        }

        SECTION("Render") {
            SECTION("simple") {
                r.StartFrame();
                font->Draw(r, 0, 128, "This is a test");
                r.EndFrame();
                r.WaitUntilDeviceIdle();
                REQUIRE(r.GetDebugMessenger()->GetErrorAndWarningCount() == 0);
            }

            SECTION("multiple frames") {
                r.StartFrame();
                font->Draw(r, 0, 128, "This is a test");
                r.EndFrame();
                r.StartFrame();
                font->Draw(r, 0, 128, "This is a test");
                r.EndFrame();
                r.StartFrame();
                font->Draw(r, 0, 128, "This is a test");
                r.EndFrame();
                r.StartFrame();
                font->Draw(r, 0, 128, "This is a test");
                r.EndFrame();
                r.StartFrame();
                font->Draw(r, 0, 128, "This is a test");
                r.EndFrame();
                r.WaitUntilDeviceIdle();
                REQUIRE(r.GetDebugMessenger()->GetErrorAndWarningCount() == 0);
            }
            
            SECTION("two fonts, multiple frames") {
                auto font1 = fm.GetFont(FONTNAME, 16);
                auto font2 = fm.GetFont(FONTNAME, 32);

                r.StartFrame();
                font1->Draw(r, 0, 128, "This is a test");
                font2->Draw(r, 0, 256, "This is also");
                font2->Draw(r, 0, 384, " a test");
                r.EndFrame();
                r.StartFrame();
                font1->Draw(r, 0, 128, "This is a test");
                font2->Draw(r, 0, 256, "This is also a test");
                r.EndFrame();
                r.StartFrame();
                font1->Draw(r, 0, 128, "This is a test");
                font2->Draw(r, 0, 256, "This is also a test");
                r.EndFrame();
                r.StartFrame();
                font1->Draw(r, 0, 128, "This is a test");
                font2->Draw(r, 0, 256, "This is also a test");
                r.EndFrame();
                r.StartFrame();
                font1->Draw(r, 0, 128, "This is a test");
                font2->Draw(r, 0, 256, "This is also a test");
                r.EndFrame();
                r.WaitUntilDeviceIdle();

                REQUIRE(r.GetDebugMessenger()->GetErrorAndWarningCount() == 0);
            }
        }
    }
    REQUIRE(r.GetDebugMessenger()->GetErrorAndWarningCount() == 0);

    glfwDestroyWindow(window);
}
