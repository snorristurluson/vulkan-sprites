//
// Created by snorri on 25.8.2018.
//

#include <gtest/gtest.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "Texture.h"

class RendererTest : public ::testing::Test {
protected:
    void SetUp() override {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        m_window = glfwCreateWindow(800, 600, "RendererTest", nullptr, nullptr);
    }

    void TearDown() override {
        glfwDestroyWindow(m_window);
    }

    GLFWwindow *m_window;
};

TEST_F(RendererTest, CanCreate) {
    Renderer r;
}

TEST_F(RendererTest, FreshInstanceIsUninitialized) {
    Renderer r;
    EXPECT_FALSE(r.IsInitialized());
    EXPECT_EQ(r.GetDebugMessenger(), nullptr);
}

TEST_F(RendererTest, Initialize_DisableValidation) {
    Renderer r;
    r.Initialize(m_window, Renderer::DISABLE_VALIDATION);
    EXPECT_TRUE(r.IsInitialized());
    EXPECT_EQ(r.GetDebugMessenger(), nullptr);
}

TEST_F(RendererTest, Initialize_EnableValidation) {
    Renderer r;
    r.Initialize(m_window, Renderer::ENABLE_VALIDATION);
    EXPECT_TRUE(r.IsInitialized());
    ASSERT_NE(r.GetDebugMessenger(), nullptr);
    EXPECT_EQ(r.GetDebugMessenger()->GetErrorAndWarningCount(), 0);
}

TEST_F(RendererTest, Initialize_MultipleCallsThrow) {
    Renderer r;
    r.Initialize(m_window, Renderer::ENABLE_VALIDATION);
    ASSERT_THROW(r.Initialize(m_window, Renderer::ENABLE_VALIDATION), std::runtime_error);
}

TEST_F(RendererTest, SetClearColor) {
    Renderer r;
    r.Initialize(m_window, Renderer::ENABLE_VALIDATION);

    glm::vec4 color {0.1f, 0.2f, 0.3f, 0.4f};
    r.SetClearColor(color);
    auto returned = r.GetClearColor();
    EXPECT_EQ(returned, color);
    EXPECT_EQ(r.GetDebugMessenger()->GetErrorAndWarningCount(), 0);
}

TEST_F(RendererTest, CreateTexture) {
    Renderer r;
    r.Initialize(m_window, Renderer::ENABLE_VALIDATION);
    auto t = r.CreateTexture("resources/texture.jpg");
    ASSERT_NE(t.get(), nullptr);
    EXPECT_EQ(t->GetWidth(), 512);
    EXPECT_EQ(t->GetHeight(), 512);
    EXPECT_EQ(r.GetDebugMessenger()->GetErrorAndWarningCount(), 0);
}

TEST_F(RendererTest, StartFrame) {
    Renderer r;
    r.Initialize(m_window, Renderer::ENABLE_VALIDATION);
    r.StartFrame();
    r.EndFrame();
    r.WaitUntilDeviceIdle();
    EXPECT_EQ(r.GetNumDrawCommands(), 0);
    EXPECT_EQ(r.GetDebugMessenger()->GetErrorAndWarningCount(), 0);
}

TEST_F(RendererTest, DrawSprite_ThrowsOutsideOfFrame) {
    Renderer r;
    r.Initialize(m_window, Renderer::ENABLE_VALIDATION);
    ASSERT_THROW(r.DrawSprite(0, 0, 100, 100), std::runtime_error);
}

TEST_F(RendererTest, DrawSprite_ThrowsOutsideOfFrame2) {
    Renderer r;
    r.Initialize(m_window, Renderer::ENABLE_VALIDATION);
    r.StartFrame();
    r.EndFrame();
    ASSERT_THROW(r.DrawSprite(0, 0, 100, 100), std::runtime_error);
    r.WaitUntilDeviceIdle();

    EXPECT_EQ(r.GetDebugMessenger()->GetErrorAndWarningCount(), 0);
}

TEST_F(RendererTest, DrawSprite) {
    Renderer r;
    r.Initialize(m_window, Renderer::ENABLE_VALIDATION);
    r.StartFrame();
    r.DrawSprite(0, 0, 100, 100);
    EXPECT_EQ(r.GetNumIndices(), 6);
    EXPECT_EQ(r.GetNumVertices(), 4);
    r.EndFrame();
    r.WaitUntilDeviceIdle();

    EXPECT_EQ(r.GetNumDrawCommands(), 1);
    EXPECT_EQ(r.GetDebugMessenger()->GetErrorAndWarningCount(), 0);
}

TEST_F(RendererTest, DrawSprite_MultipleSpritesOneDrawCommand) {
    Renderer r;
    r.Initialize(m_window, Renderer::ENABLE_VALIDATION);
    r.StartFrame();
    r.DrawSprite(0, 0, 100, 100);
    r.DrawSprite(10, 10, 100, 100);
    r.DrawSprite(20, 20, 100, 100);
    r.EndFrame();
    r.WaitUntilDeviceIdle();

    EXPECT_EQ(r.GetNumDrawCommands(), 1);
    EXPECT_EQ(r.GetDebugMessenger()->GetErrorAndWarningCount(), 0);
}

TEST_F(RendererTest, SetTexture) {
    Renderer r;
    r.Initialize(m_window, Renderer::ENABLE_VALIDATION);

    auto t = r.CreateTexture("resources/texture.jpg");

    r.StartFrame();
    r.SetTexture(t);
    r.DrawSprite(0, 0, 100, 100);
    r.EndFrame();
    r.WaitUntilDeviceIdle();

    EXPECT_EQ(r.GetDebugMessenger()->GetErrorAndWarningCount(), 0);
}

TEST_F(RendererTest, DrawSprite_OneDrawCommandPerTexture) {
    Renderer r;
    r.Initialize(m_window, Renderer::ENABLE_VALIDATION);

    auto t = r.CreateTexture("resources/texture.jpg");

    r.StartFrame();
    r.DrawSprite(0, 0, 100, 100);
    r.SetTexture(t);
    r.DrawSprite(10, 10, 100, 100);
    r.EndFrame();
    r.WaitUntilDeviceIdle();

    EXPECT_EQ(r.GetNumDrawCommands(), 2);
    EXPECT_EQ(r.GetDebugMessenger()->GetErrorAndWarningCount(), 0);
}

TEST_F(RendererTest, CreateTexture_InFrame) {
    Renderer r;
    r.Initialize(m_window, Renderer::ENABLE_VALIDATION);
    r.StartFrame();
    auto t = r.CreateTexture("resources/texture.jpg");
    r.EndFrame();
    r.WaitUntilDeviceIdle();

    ASSERT_NE(t.get(), nullptr);
    EXPECT_EQ(t->GetWidth(), 512);
    EXPECT_EQ(t->GetHeight(), 512);
    EXPECT_EQ(r.GetDebugMessenger()->GetErrorAndWarningCount(), 0);
}
