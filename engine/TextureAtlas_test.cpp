//
// Created by snorri on 7.10.2018.
//

#include "catch.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "TextureAtlas.h"


TEST_CASE("TextureAtlas") {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    auto window = glfwCreateWindow(800, 600, "TextureAtlasTest", nullptr, nullptr);

    Renderer r;
    r.Initialize(window, Renderer::ENABLE_VALIDATION);

    auto ta = r.CreateTextureAtlas(256, 256);
    REQUIRE(ta);

    SECTION("can create") {
        REQUIRE(ta->GetWidth() == 256);
        REQUIRE(ta->GetHeight() == 256);
    }

    SECTION("add a single texture") {
        auto at = ta->Add("resources/1.png");
        REQUIRE(at);

        auto tw = at->GetTextureWindow();
        REQUIRE(tw.x0 == 0.0f);
        REQUIRE(tw.y0 == 0.0f);
        REQUIRE(tw.x1 == 0.25f);
        REQUIRE(tw.y1 == 0.25f);
    }

    SECTION("add a single texture, too large") {
        auto at = ta->Add("resources/texture.jpg");
        REQUIRE(!at);
    }

    SECTION("add two textures") {
        auto at1 = ta->Add("resources/1.png");
        auto at2 = ta->Add("resources/2.png");

        REQUIRE(at1);
        REQUIRE(at2);
    }

    SECTION("add a single texture in a frame") {
        r.StartFrame();
        auto at1 = ta->Add("resources/1.png");
        r.EndFrame();
        r.WaitUntilDeviceIdle();

        REQUIRE(at1);
    }

    SECTION("add a single texture in a frame with render") {
        r.StartFrame();
        auto at1 = ta->Add("resources/1.png");
        REQUIRE(at1);

        r.SetTexture(at1);
        r.DrawSprite(0, 0, at1->GetWidth(), at1->GetHeight());
        r.EndFrame();
        r.WaitUntilDeviceIdle();
    }

    glfwDestroyWindow(window);

    REQUIRE(r.GetDebugMessenger()->GetErrorAndWarningCount() == 0);
}
