//
// Created by snorri on 8.10.2018.
//

#include "TextureAtlasApp.h"
#include "../../engine/Renderer.h"
#include "../../engine/TextureAtlas.h"

TextureAtlasApp::TextureAtlasApp() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

void TextureAtlasApp::CreateWindow(int width, int height) {
    m_window = glfwCreateWindow(width, height, "TextureAtlas", nullptr, nullptr);

    glfwGetFramebufferSize(m_window, &m_width, &m_height);

    m_width *= 2;
    m_height *= 2;

    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
}

void TextureAtlasApp::framebufferResizeCallback(GLFWwindow *window, int width, int height) {

}

void TextureAtlasApp::Run() {
    Renderer r;
    r.Initialize(m_window, Renderer::ENABLE_VALIDATION);

    auto ta = r.CreateTextureAtlas(512, 512);
    auto t1 = ta->Add("freecandybuttons/png/Back (1).png");
    auto t2 = ta->Add("freecandybuttons/png/Back (2).png");

    r.SetClearColor({0.0f, 0.0f, 1.0f, 1.0f});
    while (!glfwWindowShouldClose(m_window)) {
        if (r.StartFrame()) {
            r.SetTexture(ta);
            r.DrawSprite(0, 0, 512, 512);
            r.SetTexture(t1);
            r.DrawSprite(512, 0, 64, 64);
            r.SetTexture(t2);
            r.DrawSprite(512, 64, 64, 64);
            r.EndFrame();
        }

        glfwPollEvents();
    }
    r.WaitUntilDeviceIdle();
}
