//
// Created by snorri on 25.10.2018.
//

#include <Renderer.h>
#include <FontManager.h>
#include "TextureAtlas.h"
#include "ParticleSystem.h"
#include "ParticlesApp.h"
#include "telemetry.h"

ParticlesApp::ParticlesApp() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

void ParticlesApp::CreateWindow(int width, int height) {
    m_window = glfwCreateWindow(width, height, "Particles", nullptr, nullptr);

    glfwGetFramebufferSize(m_window, &m_width, &m_height);

    m_width *= 2;
    m_height *= 2;

    glfwSetWindowUserPointer(m_window, this);
}

void ParticlesApp::Run() {
    Renderer r;
    r.Initialize(m_window, Renderer::ENABLE_VALIDATION);

    auto ta = r.CreateTextureAtlas(512, 512);
    FontManager fm(ta);
    auto font = fm.GetFont("resources/montserrat/Montserrat-Bold.ttf", 36);

    ParticleSystem ps;

    std::default_random_engine random_engine;
    std::uniform_real_distribution<float> rng;

    for(int i = 0; i < 50; ++i) {
        auto em = ps.AddEmitter();
        em->SetEmissionRate(rng(random_engine) * 50.0f + 10.0f);
        em->SetPosition({rng(random_engine) * 800, rng(random_engine) * 600});
        em->SetLifespan(rng(random_engine) * 5.0f + 1.0f);
    }

    r.SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});
    while (!glfwWindowShouldClose(m_window)) {
        tmZone(0, 0, "main loop");

        glfwPollEvents();

        float td = 1.0f / 60.0f;
        ps.Update(td);

        if(r.StartFrame()) {
            ps.Render(r);

            auto n = ps.GetNumParticles();
            char buffer[32];
            sprintf(buffer, "Num particles: %d", n);
            r.SetColor({1.0f, 0.5f, 0.5f, 1});
            font->Draw(r, 32, 32, buffer);
            r.EndFrame();
        }
    }
    r.WaitUntilDeviceIdle();

}
