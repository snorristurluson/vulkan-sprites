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
    m_window = glfwCreateWindow(width, height, "TextureAtlas", nullptr, nullptr);

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
    auto font = fm.GetFont("/usr/share/fonts/truetype/freefont/FreeMonoBoldOblique.ttf", 144);
    r.SetClearColor({0.0f, 0.0f, 1.0f, 1.0f});
    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
        if(r.StartFrame()) {
            r.SetColor({0.0f, 1.0f, 0.0f, 1.0f});
            font->Draw(r, 0, 128, "This is a test");

            r.SetTexture(ta);
            r.DrawSprite(2048, 0, 2048, 2048);

            r.EndFrame();
        }
    }
    r.WaitUntilDeviceIdle();

}
