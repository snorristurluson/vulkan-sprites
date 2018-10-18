//
// Created by snorri on 14.10.2018.
//

#include <Logger.h>
#include <GLFW/glfw3.h>
#include "Font.h"
#include "GameOver.h"
#include "telemetry.h"

static auto logger = GetLogger("mainmenu");

GameOver::GameOver(App *a) : AppState(a) {

}

void GameOver::Init(Renderer &r) {
    m_titleFont = m_app->GetFont(48);
}

void GameOver::Enter(Renderer &r) {
    tmFunction(0, 0);

    logger->debug(__PRETTY_FUNCTION__);
    m_frameCounter = 0;

}

void GameOver::Render(Renderer &r) {
    tmFunction(0, 0);

    m_frameCounter++;

    m_titleFont->Draw(r, 32, 128, "GAME OVER");
}

void GameOver::Exit(Renderer &r) {
    tmFunction(0, 0);

    logger->debug(__PRETTY_FUNCTION__);
}

void GameOver::HandleKey(int key, int scancode, int action, int mods) {
    if(action == GLFW_PRESS) {
        m_app->ChangeState(MAIN_MENU);
    }

}
