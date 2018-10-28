//
// Created by snorri on 26.10.2018.
//

#include "CurvesApp.h"
#include "Curve.h"
#include "Renderer.h"
#include "FontManager.h"
#include "telemetry.h"

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

CurvesApp::CurvesApp() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

void CurvesApp::CreateWindow(int width, int height) {
    m_window = glfwCreateWindow(width, height, "Curves", nullptr, nullptr);

    glfwGetFramebufferSize(m_window, &m_width, &m_height);

    m_width *= 2;
    m_height *= 2;

    glfwSetWindowUserPointer(m_window, this);
}

void CurvesApp::Run() {
    Renderer r;
    r.Initialize(m_window, Renderer::ENABLE_VALIDATION);

    auto ta = r.CreateTextureAtlas(512, 512);
    FontManager fm(ta);
    auto font = fm.GetFont("resources/montserrat/Montserrat-Bold.ttf", 36);

    float time = 0.0f;

    Curve<float> c1;
    c1.AddKey(0.0f, 42.0f);
    c1.AddKey(5.0f, 3.14f);
    c1.SetExtrapolationType(CurveExtrapolation::CYCLE);

    Curve<glm::vec2> c2;
    c2.AddKey(0.0f, {100, 100});
    c2.AddKey(1.0f, {300, 100});
    c2.AddKey(2.0f, {300, 300});
    c2.AddKey(3.0f, {100, 300});
    c2.AddKey(4.0f, {100, 100});
    c2.SetExtrapolationType(CurveExtrapolation::CYCLE);

    Curve<glm::vec4> c3;
    c3.AddKey(0.0f, {0, 0, 0, 1});
    c3.AddKey(2.0f, {1, 0, 0, 1});
    c3.AddKey(3.0f, {0, 0, 1, 1});
    c3.SetExtrapolationType(CurveExtrapolation::MIRROR);

    r.SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});
    while (!glfwWindowShouldClose(m_window)) {
        tmZone(0, 0, "main loop");

        glfwPollEvents();

        float td = 1.0f / 60.0f;
        if(r.StartFrame()) {
            char buffer[256];

            sprintf(buffer, "%4.2f", time);
            r.SetColor({1,1,1,1});
            font->Draw(r, 20, 40, buffer);

            sprintf(buffer, "%4.2f", c1.GetValue(time));
            r.SetColor({1,1,1,1});
            font->Draw(r, 20, 80, buffer);

            r.SetTexture(nullptr);
            for(int i = 0; i < 32; ++i) {
                r.SetColor(c3.GetValue(time + i*0.01f));
                r.DrawSprite(20 + i*20, 300, 20, 200);
            }

            auto pos = c2.GetValue(time);
            r.SetTexture(nullptr);
            r.SetColor({1,1,1,1});
            r.DrawSprite(pos.x, pos.y, 16, 16);

            time += td;
            r.EndFrame();
        }
    }
    r.WaitUntilDeviceIdle();

}
