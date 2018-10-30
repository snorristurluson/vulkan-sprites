//
// Created by snorri on 25.8.2018.
//

#include "catch.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "Texture.h"
#include "Vertex.h"

TEST_CASE("Renderer basics") {
    SECTION("can create") {
        Renderer r;
    }

    SECTION("fresh instance is uninitialized") {
        Renderer r;
        REQUIRE(!r.IsInitialized());
        REQUIRE(!r.GetDebugMessenger());
    }
}

TEST_CASE("Renderer") {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    auto window = glfwCreateWindow(800, 600, "TextureAtlasTest", nullptr, nullptr);

    SECTION("Initialize") {
        SECTION("with validation") {
            Renderer r;
            r.Initialize(window, Renderer::ENABLE_VALIDATION);
            REQUIRE(r.IsInitialized());
            REQUIRE(r.GetDebugMessenger());
            REQUIRE(r.GetDebugMessenger()->GetErrorAndWarningCount() == 0);
        }

        SECTION("without validation") {
            Renderer r;
            r.Initialize(window, Renderer::DISABLE_VALIDATION);
            REQUIRE(r.IsInitialized());
            REQUIRE(!r.GetDebugMessenger());
        }

        SECTION("multiple calls throw") {
            Renderer r;
            r.Initialize(window, Renderer::ENABLE_VALIDATION);
            REQUIRE_THROWS_AS(r.Initialize(window, Renderer::ENABLE_VALIDATION), std::runtime_error);
        }
    }

    Renderer r;
    r.Initialize(window, Renderer::ENABLE_VALIDATION);

    SECTION("SetClearColor") {
        glm::vec4 color {0.1f, 0.2f, 0.3f, 0.4f};
        r.SetClearColor(color);
        auto returned = r.GetClearColor();
        REQUIRE(returned == color);

        r.WaitUntilDeviceIdle();
        REQUIRE(r.GetDebugMessenger()->GetErrorAndWarningCount() == 0);
    }

    SECTION("CreateTexture") {
        SECTION("simple case") {
            auto t = r.CreateTexture("resources/texture.jpg");
            REQUIRE(t);
            REQUIRE(t->GetWidth() == 512);
            REQUIRE(t->GetHeight() == 512);

            r.WaitUntilDeviceIdle();
            REQUIRE(r.GetDebugMessenger()->GetErrorAndWarningCount() == 0);
        }

        SECTION("in frame") {
            r.StartFrame();
            auto t = r.CreateTexture("resources/texture.jpg");
            r.EndFrame();

            REQUIRE(t);
            REQUIRE(t->GetWidth() == 512);
            REQUIRE(t->GetHeight() == 512);

            r.WaitUntilDeviceIdle();
            REQUIRE(r.GetDebugMessenger()->GetErrorAndWarningCount() == 0);
        }
    }

    SECTION("StartFrame") {
        r.StartFrame();
        r.EndFrame();
        REQUIRE(r.GetNumDrawCommands() == 0);

        r.WaitUntilDeviceIdle();
        REQUIRE(r.GetDebugMessenger()->GetErrorAndWarningCount() == 0);
    }

    SECTION("DrawSprite") {
        SECTION("throws outside of frame") {
            REQUIRE_THROWS_AS(r.DrawSprite(0, 0, 100, 100), std::runtime_error);
        }

        SECTION("throws outside of frame (after EndFrame)") {
            r.StartFrame();
            r.EndFrame();
            REQUIRE_THROWS_AS(r.DrawSprite(0, 0, 100, 100), std::runtime_error);

            r.WaitUntilDeviceIdle();
            REQUIRE(r.GetDebugMessenger()->GetErrorAndWarningCount() == 0);
        }

        SECTION("simple case") {
            r.StartFrame();
            r.DrawSprite(0, 0, 100, 100);

            REQUIRE(r.GetNumIndices() == 6);
            REQUIRE(r.GetNumVertices() == 4);

            r.EndFrame();
            REQUIRE(r.GetNumDrawCommands() == 1);

            r.WaitUntilDeviceIdle();
            REQUIRE(r.GetDebugMessenger()->GetErrorAndWarningCount() == 0);
        }

        SECTION("multiple sprites, one draw command") {
            r.StartFrame();
            r.DrawSprite(0, 0, 100, 100);
            r.DrawSprite(10, 10, 100, 100);
            r.DrawSprite(20, 20, 100, 100);
            r.EndFrame();

            REQUIRE(r.GetNumDrawCommands() == 1);

            r.WaitUntilDeviceIdle();
            REQUIRE(r.GetDebugMessenger()->GetErrorAndWarningCount() == 0);
        }

        SECTION("one draw command per texture switch") {
            auto t1 = r.CreateTexture("resources/1.png");
            auto t2 = r.CreateTexture("resources/2.png");

            r.StartFrame();
            r.SetTexture(t1);
            r.DrawSprite(0, 0, 100, 100);
            r.DrawSprite(0, 100, 100, 100);
            r.SetTexture(t2);
            r.DrawSprite(10, 10, 100, 100);
            r.DrawSprite(10, 110, 100, 100);
            r.SetTexture(t1);
            r.DrawSprite(0, 0, 100, 100);
            r.DrawSprite(0, 100, 100, 100);
            r.EndFrame();

            REQUIRE(r.GetNumDrawCommands() == 3);

            r.WaitUntilDeviceIdle();
            REQUIRE(r.GetDebugMessenger()->GetErrorAndWarningCount() == 0);
        }
    }

    SECTION("DrawTriangles") {
        SECTION("simple case") {
            std::array<uint16_t, 6> indices {0, 1, 3,  3, 1, 2};
            std::array<Vertex, 4> vertices {
                {
                    {{  0,   0}, {1, 1, 1, 1}, {0, 0}},
                    {{  0, 100}, {1, 1, 1, 1}, {0, 1}},
                    {{100, 100}, {1, 1, 1, 1}, {1, 1}},
                    {{100,   0}, {1, 1, 1, 1}, {1, 0}},
                }
            };

            r.StartFrame();
            r.DrawTriangles(indices.data(), indices.size(), vertices.data(), vertices.size());
            r.EndFrame();

            REQUIRE(r.GetNumDrawCommands() == 1);

            r.WaitUntilDeviceIdle();
            REQUIRE(r.GetDebugMessenger()->GetErrorAndWarningCount() == 0);
        }
    }

    SECTION("SetTexture") {
        SECTION("simple case") {
            auto t = r.CreateTexture("resources/texture.jpg");

            r.StartFrame();
            r.SetTexture(t);
            r.DrawSprite(0, 0, 100, 100);
            r.EndFrame();

            r.WaitUntilDeviceIdle();
            REQUIRE(r.GetDebugMessenger()->GetErrorAndWarningCount() == 0);
        }

        SECTION("nullptr") {
            r.StartFrame();
            r.SetTexture(nullptr);
            r.DrawSprite(0, 0, 100, 100);
            r.EndFrame();

            r.WaitUntilDeviceIdle();
            REQUIRE(r.GetDebugMessenger()->GetErrorAndWarningCount() == 0);
        }
    }
    SECTION("Blend mode") {
        SECTION("initial blend mode is none") {
            REQUIRE(r.GetBlendMode() == BM_NONE);
        }

        SECTION("SetBlendMode") {
            r.SetBlendMode(BM_ADD);
            REQUIRE(r.GetBlendMode() == BM_ADD);
        }
    }


    r.WaitUntilDeviceIdle();
    REQUIRE(r.GetDebugMessenger()->GetErrorAndWarningCount() == 0);

    glfwDestroyWindow(window);
}
