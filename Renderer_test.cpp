//
// Created by snorri on 25.8.2018.
//

#include <gtest/gtest.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "Texture.h"

// TODO: Hook into validation to allow tests to check for validation messages

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
}

TEST_F(RendererTest, Initialize_DisableValidation) {
    Renderer r;
    r.Initialize(m_window, Renderer::DISABLE_VALIDATION);
    EXPECT_TRUE(r.IsInitialized());
}

TEST_F(RendererTest, Initialize_EnableValidation) {
    Renderer r;
    r.Initialize(m_window, Renderer::ENABLE_VALIDATION);
    EXPECT_TRUE(r.IsInitialized());
}

TEST_F(RendererTest, Initialize_MultipleCallsThrow) {
    Renderer r;
    r.Initialize(m_window, Renderer::ENABLE_VALIDATION);
    ASSERT_THROW(r.Initialize(m_window, Renderer::ENABLE_VALIDATION), std::runtime_error);
}

TEST_F(RendererTest, CreateTexture) {
    Renderer r;
    r.Initialize(m_window, Renderer::ENABLE_VALIDATION);
    auto t = r.CreateTexture("resources/texture.jpg");
    ASSERT_NE(t.get(), nullptr);
    EXPECT_EQ(t->GetWidth(), 512);
    EXPECT_EQ(t->GetHeight(), 512);
}

TEST_F(RendererTest, StartFrame) {
    Renderer r;
    r.Initialize(m_window, Renderer::ENABLE_VALIDATION);
    r.StartFrame();
    r.EndFrame();
    r.WaitUntilDeviceIdle();

    // TODO ensure validation log is empty
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

    // TODO ensure validation log is empty
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

    // TODO ensure validation log is empty
}