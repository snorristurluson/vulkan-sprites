//
// Created by snorri on 14.10.2018.
//

#include <Logger.h>
#include <GLFW/glfw3.h>
#include "Font.h"
#include "Gameplay.h"
#include "telemetry.h"

static auto logger = GetLogger("gameplay");

Gameplay::Gameplay(App *a) : AppState(a) {

}

void Gameplay::Init(Renderer &r) {
    m_titleFont = m_app->GetFont("/usr/share/fonts/truetype/freefont/FreeMonoBoldOblique.ttf", 96);

}

void Gameplay::Enter(Renderer &r) {
    tmFunction(0, 0);

    logger->debug(__PRETTY_FUNCTION__);
    m_frameCounter = 0;

}

void Gameplay::Render(Renderer &r) {
    tmFunction(0, 0);

    m_frameCounter++;

    m_titleFont->Draw(r, 32, 128, "Gameplay");

}

void Gameplay::Exit(Renderer &r) {
    tmFunction(0, 0);

    logger->debug(__PRETTY_FUNCTION__);
}

void Gameplay::HandleKey(int key, int scancode, int action, int mods) {
    tmFunction(0, 0);

    if(action == GLFW_PRESS) {
        m_app->ChangeState(GAME_OVER);
    }

}
