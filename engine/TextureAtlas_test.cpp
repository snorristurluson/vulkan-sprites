//
// Created by snorri on 7.10.2018.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <gtest/gtest.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "TextureAtlas.h"

class TextureAtlasTest : public ::testing::Test {
protected:
    void SetUp() override {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        m_window = glfwCreateWindow(800, 600, "TextureAtlasTest", nullptr, nullptr);
        m_renderer.Initialize(m_window, Renderer::ENABLE_VALIDATION);
    }

    void TearDown() override {
        glfwDestroyWindow(m_window);
    }

    GLFWwindow *m_window{};
    Renderer m_renderer;
};

TEST_F(TextureAtlasTest, CanCreate) {
    auto ta = m_renderer.CreateTextureAtlas(256, 256);

    ASSERT_NE(ta, nullptr);
    EXPECT_EQ(ta->GetWidth(), 256);
    EXPECT_EQ(ta->GetHeight(), 256);
    EXPECT_EQ(m_renderer.GetDebugMessenger()->GetErrorAndWarningCount(), 0);
}

TEST_F(TextureAtlasTest, Add_Single) {
    auto ta = m_renderer.CreateTextureAtlas(256, 256);

    auto at = ta->Add("resources/1.png");

    ASSERT_NE(at, nullptr);

    auto tw = at->GetTextureWindow();
    EXPECT_EQ(tw.x0, 0.0f);
    EXPECT_EQ(tw.y0, 0.0f);
    EXPECT_EQ(tw.x1, 0.25f);
    EXPECT_EQ(tw.y1, 0.25f);
    EXPECT_EQ(m_renderer.GetDebugMessenger()->GetErrorAndWarningCount(), 0);
}

TEST_F(TextureAtlasTest, Add_SingleTooLarge) {
    auto ta = m_renderer.CreateTextureAtlas(16, 16);

    auto at = ta->Add("resources/1.png");

    EXPECT_EQ(at, nullptr);
    EXPECT_EQ(m_renderer.GetDebugMessenger()->GetErrorAndWarningCount(), 0);
}

TEST_F(TextureAtlasTest, Add_Two) {
    auto ta = m_renderer.CreateTextureAtlas(256, 256);

    auto at1 = ta->Add("resources/1.png");
    auto at2 = ta->Add("resources/2.png");

    ASSERT_NE(at1, nullptr);
    ASSERT_NE(at2, nullptr);

    EXPECT_EQ(m_renderer.GetDebugMessenger()->GetErrorAndWarningCount(), 0);
}

TEST_F(TextureAtlasTest, Add_SingleInFrame) {
    auto ta = m_renderer.CreateTextureAtlas(256, 256);

    m_renderer.StartFrame();
    auto at1 = ta->Add("resources/1.png");
    m_renderer.EndFrame();
    m_renderer.WaitUntilDeviceIdle();

    ASSERT_NE(at1, nullptr);

    EXPECT_EQ(m_renderer.GetDebugMessenger()->GetErrorAndWarningCount(), 0);
}

TEST_F(TextureAtlasTest, Add_SingleInFrameWithRender) {
    auto ta = m_renderer.CreateTextureAtlas(256, 256);

    m_renderer.StartFrame();
    auto at1 = ta->Add("resources/1.png");
    m_renderer.SetTexture(at1);
    m_renderer.DrawSprite(0, 0, at1->GetWidth(), at1->GetHeight());
    m_renderer.EndFrame();
    m_renderer.WaitUntilDeviceIdle();

    ASSERT_NE(at1, nullptr);

    EXPECT_EQ(m_renderer.GetDebugMessenger()->GetErrorAndWarningCount(), 0);
}

#pragma clang diagnostic pop