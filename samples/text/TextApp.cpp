//
// Created by snorri on 10.10.2018.
//

#include "TextApp.h"
#include "../../engine/Renderer.h"
#include "../../engine/TextureAtlas.h"
#include "../../engine/FontManager.h"

TextApp::TextApp() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

void TextApp::CreateWindow(int width, int height) {
    m_window = glfwCreateWindow(width, height, "Text", nullptr, nullptr);

    glfwGetFramebufferSize(m_window, &m_width, &m_height);

    m_width *= 2;
    m_height *= 2;

    glfwSetWindowUserPointer(m_window, this);
}

void TextApp::Run() {
    Renderer r;
    r.Initialize(m_window, Renderer::ENABLE_VALIDATION);

    auto ta = r.CreateTextureAtlas(512, 512);
    FontManager fm(ta);
    auto font = fm.GetFont("resources/montserrat/Montserrat-Bold.ttf", 36);
    r.SetClearColor({1.0f, 1.0f, 1.0f, 1.0f});
    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
        if(r.StartFrame()) {
            r.SetColor({0.0f, 0.0f, 0.0f, 1.0f});
            font->Draw(r, 80, 100, "This is a test");

            r.SetColor({1.0f, 0.0f, 0.0f, 1.0f});
            font->Draw(r, 30, 140, "This is a red test");

            r.SetColor({0.0f, 0.0f, 1.0f, 1.0f});
            font->Draw(r, 30, 180, "The quick brown fox");
            font->Draw(r, 60, 220, "jumps over the lazy dog");

            r.SetTexture(ta);
            r.DrawSprite(2048, 0, 2048, 2048);

            r.EndFrame();
        }
    }
    r.WaitUntilDeviceIdle();

}
