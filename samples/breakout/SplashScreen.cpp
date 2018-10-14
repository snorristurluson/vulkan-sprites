//
// Created by snorri on 12.10.2018.
//

#include <Logger.h>
#include "Renderer.h"
#include "SplashScreen.h"
#include "telemetry.h"

static auto logger = GetLogger("splashscreen");

SplashScreen::SplashScreen(App *a) : AppState(a) {}

void SplashScreen::Init(Renderer &r) {
    m_font = m_app->GetFont("/usr/share/fonts/truetype/freefont/FreeMonoBoldOblique.ttf", 144);
}

void SplashScreen::Enter(Renderer &r) {
    tmFunction(0, 0);

    logger->debug(__PRETTY_FUNCTION__);

    m_frameCounter = 0;
}

void SplashScreen::Render(Renderer &r) {
    tmFunction(0, 0);

    m_frameCounter++;

    m_font->Draw(r, 32, 256, "Vulkan BreakOut");
}

void SplashScreen::Exit(Renderer &r) {
    tmFunction(0, 0);

    logger->debug(__PRETTY_FUNCTION__);
}

void SplashScreen::HandleKey(int key, int scancode, int action, int mods) {
    tmFunction(0, 0);

    if(action == GLFW_PRESS) {
        if(key == GLFW_KEY_ESCAPE) {
            m_app->ChangeState(QUIT);
        } else {
            m_app->ChangeState(MAIN_MENU);
        }
    }
}
